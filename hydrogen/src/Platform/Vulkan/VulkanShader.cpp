#include <Hydrogen/Platform/Vulkan/VulkanShader.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanRenderDevice.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanSwapChain.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanFramebuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanBuffer.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanTexture.hpp>
#include <Hydrogen/Platform/Vulkan/VulkanCommandBuffer.hpp>
#include <Hydrogen/Renderer/ShaderCompiler.hpp>
#include <Hydrogen/Renderer/Buffer.hpp>
#include <Hydrogen/Core/Base.hpp>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>

using namespace Hydrogen::Vulkan;
using namespace Hydrogen;

namespace Hydrogen::Vulkan::Utils {
static VkFormat ShaderDataTypeToVkFormat(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::None:
      HY_INVOKE_ERROR("Invalid ShaderDataType value (ShaderDataType::None)!");
    case ShaderDataType::Float:
      return VK_FORMAT_R32_SFLOAT;
      break;
    case ShaderDataType::Float2:
      return VK_FORMAT_R32G32_SFLOAT;
      break;
    case ShaderDataType::Float3:
      return VK_FORMAT_R32G32B32_SFLOAT;
      break;
    case ShaderDataType::Float4:
      return VK_FORMAT_R32G32B32A32_SFLOAT;
      break;
    case ShaderDataType::Mat3:
      HY_INVOKE_ERROR("ShaderDataType::Mat3 is not supported for vulkan shader input!");
      break;
    case ShaderDataType::Mat4:
      HY_INVOKE_ERROR("ShaderDataType::Mat4 is not supported for vulkan shader input!");
      break;
    case ShaderDataType::Int:
      return VK_FORMAT_R32_SINT;
      break;
    case ShaderDataType::Int2:
      return VK_FORMAT_R32G32_SINT;
      break;
    case ShaderDataType::Int3:
      return VK_FORMAT_R32G32B32_SINT;
      break;
    case ShaderDataType::Int4:
      return VK_FORMAT_R32G32B32A32_SINT;
      break;
    case ShaderDataType::Bool:
      HY_INVOKE_ERROR("ShaderDataType::Bool is not supported for vulkan shader input!");
      break;
    default:
      HY_INVOKE_ERROR("Invalid ShaderDataType value!");
  }
}

static VkShaderStageFlags ShaderStageToVkShaderStageFlags(ShaderStage stage) {
  switch (stage) {
    case ShaderStage::VertexShader:
      return VK_SHADER_STAGE_VERTEX_BIT;
      break;
    case ShaderStage::PixelShader:
      return VK_SHADER_STAGE_FRAGMENT_BIT;
      break;
    case ShaderStage::GeometryShader:
      return VK_SHADER_STAGE_GEOMETRY_BIT;
      break;
    default:
      HY_INVOKE_ERROR("Invalid ShaderStage value!");
  }
}
} // namespace Hydrogen::Vulkan::Utils

VulkanShader::VulkanShader(const ReferencePointer<RenderDevice>& renderDevice, const ReferencePointer<SwapChain>& swapChain, const ReferencePointer<Framebuffer>& framebuffer,
                           const BufferLayout& vertexLayout, ShaderDependencyGraph dependencyGraph, const String& name, const DynamicArray<uint32_t>& vertexSrc,
                           const DynamicArray<uint32_t>& fragmentSrc, const DynamicArray<uint32_t>& geometrySrc)
    : m_Name(name),
      m_HasDependencies(dependencyGraph.Dependencies.size() > 0),
      m_RenderDevice(std::dynamic_pointer_cast<VulkanRenderDevice>(renderDevice)),
      m_SwapChain(std::dynamic_pointer_cast<VulkanSwapChain>(swapChain)),
      m_Framebuffer(std::dynamic_pointer_cast<VulkanFramebuffer>(framebuffer)) {
  ZoneScoped;

  m_DescriptorPool = VK_NULL_HANDLE;
  m_DescriptorSet = VK_NULL_HANDLE;
  m_DescriptorSetLayout = VK_NULL_HANDLE;

  if (m_HasDependencies) {
    DynamicArray<VkDescriptorSetLayoutBinding> m_DescriptorSetLayoutBindings(dependencyGraph.Dependencies.size());
    uint32_t numUniformBuffers = 0;
    uint32_t numTextures = 0;

    for (size_t i = 0; i < dependencyGraph.Dependencies.size(); i++) {
      VkDescriptorSetLayoutBinding layoutBinding{};
      layoutBinding.binding = dependencyGraph.Dependencies[i].Location;
      layoutBinding.descriptorCount = 1;
      layoutBinding.stageFlags = Utils::ShaderStageToVkShaderStageFlags(dependencyGraph.Dependencies[i].Stage);
      layoutBinding.pImmutableSamplers = nullptr;

      switch (dependencyGraph.Dependencies[i].Type) {
        case ShaderDependencyType::UniformBuffer:
          numUniformBuffers++;
          layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          break;
        case ShaderDependencyType::Texture:
          numTextures++;
          layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          break;
        default:
          HY_INVOKE_ERROR("Invalid ShaderDependencyType value!");
          break;
      }

      m_DescriptorSetLayoutBindings[i] = layoutBinding;
    }

    VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo{};
    descriptorLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayoutCreateInfo.bindingCount = static_cast<uint32_t>(m_DescriptorSetLayoutBindings.size());
    descriptorLayoutCreateInfo.pBindings = m_DescriptorSetLayoutBindings.data();

    VK_CHECK_ERROR(vkCreateDescriptorSetLayout(m_RenderDevice->GetDevice(), &descriptorLayoutCreateInfo, nullptr, &m_DescriptorSetLayout),
                   "Failed to create Vulkan descriptor set layout");

    DynamicArray<VkDescriptorPoolSize> descriptorPoolSizes;
    if (numUniformBuffers > 0) {
      VkDescriptorPoolSize poolSize{};
      poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSize.descriptorCount = numUniformBuffers;

      descriptorPoolSizes.push_back(poolSize);
    }

    if (numTextures > 0) {
      VkDescriptorPoolSize poolSize{};
      poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
      poolSize.descriptorCount = numTextures;

      descriptorPoolSizes.push_back(poolSize);
    }

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(descriptorPoolSizes.size());
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
    descriptorPoolCreateInfo.maxSets = MAX_FRAMES_IN_FLIGHT;

    VK_CHECK_ERROR(vkCreateDescriptorPool(m_RenderDevice->GetDevice(), &descriptorPoolCreateInfo, nullptr, &m_DescriptorPool), "Failed to create vulkan descriptor pool!");
  }

  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = vertexLayout.GetStride();
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  const auto& elements = vertexLayout.GetElements();
  DynamicArray<VkVertexInputAttributeDescription> attributeDescriptions(elements.size());
  for (size_t i = 0; i < elements.size(); i++) {
    HY_ASSERT(!elements[i].Normalized, "Normalization of vertex input is not supported in vulkan!");
    attributeDescriptions[i].binding = 0;
    attributeDescriptions[i].location = static_cast<uint32_t>(i);
    attributeDescriptions[i].format = Utils::ShaderDataTypeToVkFormat(elements[i].Type);
    attributeDescriptions[i].offset = static_cast<uint32_t>(elements[i].Offset);
  }

  DynamicArray<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
  shaderStageCreateInfos.reserve(3);

  m_VertexShaderModule = VK_NULL_HANDLE;
  m_FragmentShaderModule = VK_NULL_HANDLE;
  m_GeometryShaderModule = VK_NULL_HANDLE;

  if (vertexSrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = vertexSrc.size() * sizeof(uint32_t);
    createInfo.pCode = reinterpret_cast<const uint32_t*>(vertexSrc.data());

    VK_CHECK_ERROR(vkCreateShaderModule(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_VertexShaderModule), "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = m_VertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(vertexShaderStageInfo);
  }

  if (fragmentSrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fragmentSrc.size() * sizeof(uint32_t);
    createInfo.pCode = reinterpret_cast<const uint32_t*>(fragmentSrc.data());

    VK_CHECK_ERROR(vkCreateShaderModule(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_FragmentShaderModule), "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = m_FragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(fragmentShaderStageInfo);
  }

  if (geometrySrc.size() > 0) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = geometrySrc.size() * sizeof(uint32_t);
    createInfo.pCode = reinterpret_cast<const uint32_t*>(geometrySrc.data());

    VK_CHECK_ERROR(vkCreateShaderModule(m_RenderDevice->GetDevice(), &createInfo, nullptr, &m_GeometryShaderModule), "Failed to create vulkan shader module!");

    VkPipelineShaderStageCreateInfo geometryShaderStageInfo{};
    geometryShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
    geometryShaderStageInfo.module = m_GeometryShaderModule;
    geometryShaderStageInfo.pName = "main";

    shaderStageCreateInfos.push_back(geometryShaderStageInfo);
  }

  DynamicArray<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)m_SwapChain->GetExtent().width;
  viewport.height = (float)m_SwapChain->GetExtent().height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = m_SwapChain->GetExtent();

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.pViewports = &viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &scissor;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;  // Optional
  rasterizer.depthBiasClamp = 0.0f;           // Optional
  rasterizer.depthBiasSlopeFactor = 0.0f;     // Optional

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;           // Optional
  multisampling.pSampleMask = nullptr;             // Optional
  multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
  multisampling.alphaToOneEnable = VK_FALSE;       // Optional

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_FALSE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
  colorBlendAttachment.blendEnable = VK_TRUE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;  // Optional
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;  // Optional
  colorBlending.blendConstants[1] = 0.0f;  // Optional
  colorBlending.blendConstants[2] = 0.0f;  // Optional
  colorBlending.blendConstants[3] = 0.0f;  // Optional

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;

  if (m_HasDependencies) {
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
  }

  VK_CHECK_ERROR(vkCreatePipelineLayout(m_RenderDevice->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout), "Failed to create vulkan pipeline layout!");

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size());
  pipelineInfo.pStages = shaderStageCreateInfos.data();
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = nullptr;  // Optional
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = m_PipelineLayout;
  pipelineInfo.renderPass = m_Framebuffer->GetRenderPass();
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;  // Optional
  pipelineInfo.basePipelineIndex = -1;               // Optional

  VK_CHECK_ERROR(vkCreateGraphicsPipelines(m_RenderDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline), "Failed to create vulkan pipeline!");

  if (m_HasDependencies) {
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_DescriptorSetLayout;

    VK_CHECK_ERROR(vkAllocateDescriptorSets(m_RenderDevice->GetDevice(), &allocInfo, &m_DescriptorSet), "Failed to create vulkan descriptor set!");

    for (size_t i = 0; i < dependencyGraph.Dependencies.size(); i++) {
      switch (dependencyGraph.Dependencies[i].Type) {
        case ShaderDependencyType::UniformBuffer: {
          ReferencePointer<VulkanUniformBuffer> uniformBuffer = std::dynamic_pointer_cast<VulkanUniformBuffer>(dependencyGraph.Dependencies[i].UniformBuffer);

          VkDescriptorBufferInfo bufferInfo{};
          bufferInfo.buffer = uniformBuffer->GetBuffer();
          bufferInfo.offset = 0;
          bufferInfo.range = static_cast<uint32_t>(uniformBuffer->GetSize());

          VkWriteDescriptorSet descriptorWrite{};
          descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptorWrite.dstSet = m_DescriptorSet;
          descriptorWrite.dstBinding = dependencyGraph.Dependencies[i].Location;
          descriptorWrite.dstArrayElement = 0;
          descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          descriptorWrite.descriptorCount = 1;
          descriptorWrite.pBufferInfo = &bufferInfo;

          vkUpdateDescriptorSets(m_RenderDevice->GetDevice(), 1, &descriptorWrite, 0, nullptr);
          break;
        }
        case ShaderDependencyType::Texture: {
          ReferencePointer<VulkanTexture2D> texture = std::dynamic_pointer_cast<VulkanTexture2D>(dependencyGraph.Dependencies[i].Texture);

          VkDescriptorImageInfo imageInfo{};
          imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          imageInfo.imageView = texture->GetImageView();
          imageInfo.sampler = texture->GetSampler();

          VkWriteDescriptorSet descriptorWrite{};
          descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptorWrite.dstSet = m_DescriptorSet;
          descriptorWrite.dstBinding = dependencyGraph.Dependencies[i].Location;
          descriptorWrite.dstArrayElement = 0;
          descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptorWrite.descriptorCount = 1;
          descriptorWrite.pImageInfo = &imageInfo;

          vkUpdateDescriptorSets(m_RenderDevice->GetDevice(), 1, &descriptorWrite, 0, nullptr);
          break;
        }
        default:
          HY_INVOKE_ERROR("Invalid ShaderDependencyType value!");
          break;
      }
    }
  }
}

VulkanShader::~VulkanShader() {
  ZoneScoped;
  
  if (m_HasDependencies)
    vkDestroyDescriptorSetLayout(m_RenderDevice->GetDevice(), m_DescriptorSetLayout, nullptr);
  vkDestroyPipeline(m_RenderDevice->GetDevice(), m_Pipeline, nullptr);
  vkDestroyPipelineLayout(m_RenderDevice->GetDevice(), m_PipelineLayout, nullptr);

  if (m_VertexShaderModule != VK_NULL_HANDLE) vkDestroyShaderModule(m_RenderDevice->GetDevice(), m_VertexShaderModule, nullptr);
  if (m_FragmentShaderModule != VK_NULL_HANDLE) vkDestroyShaderModule(m_RenderDevice->GetDevice(), m_FragmentShaderModule, nullptr);
  if (m_GeometryShaderModule != VK_NULL_HANDLE) vkDestroyShaderModule(m_RenderDevice->GetDevice(), m_GeometryShaderModule, nullptr);

  if (m_HasDependencies)
    vkDestroyDescriptorPool(m_RenderDevice->GetDevice(), m_DescriptorPool, nullptr);
}

void VulkanShader::Bind(const ReferencePointer<CommandBuffer>& commandBuffer) const {
  ZoneScoped;
  auto vulkanCommandBuffer = std::dynamic_pointer_cast<VulkanCommandBuffer>(commandBuffer)->GetCommandBuffer();

  if (m_HasDependencies)
    vkCmdBindDescriptorSets(vulkanCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSet, 0, nullptr);
  vkCmdBindPipeline(vulkanCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
}
