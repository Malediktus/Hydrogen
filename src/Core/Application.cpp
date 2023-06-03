#include <Hydrogen/Core/Application.hpp>
#include <Hydrogen/Core/Task.hpp>
#include <Hydrogen/Assets/AssetManager.hpp>

using namespace Hydrogen;

Application::Application() {
    Console = NewReferencePointer<Logger>("APP", Logger::LogLevel::Info);
    SystemLogger::Init();
}

void Application::Run() {
    OnSetup();
    AppWindow = Window::Create(ApplicationInfo.Name, ApplicationInfo.WindowSize.x, ApplicationInfo.WindowSize.y);

    ProjectInformation clientProject;
    clientProject.ProjectName = ApplicationInfo.Name;
    clientProject.ProjectVersion = ApplicationInfo.Version;

    ProjectInformation engineProject;
    engineProject.ProjectName = ApplicationInfo.Name;
    engineProject.ProjectVersion = ApplicationInfo.Version;

    m_RenderContext = Context::Create(AppWindow, clientProject, engineProject);

    Renderer::SetContext(m_RenderContext);
    RenderCommand::Init();
    RenderCommand::ConfigureAntiAliasing(true);

    AssetManager::Init();

    // ReferencePointer<ShaderAsset> defaultShader = AssetManager::Get<ShaderAsset>("assets/BlinnPhong.glsl");

    CurrentScene = NewReferencePointer<Scene>();

    // Register event callbacks
    EventDispatcher::Subscribe<WindowResizeEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<WindowCloseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyPressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<KeyRepeatEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseMoveEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MousePressEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseReleaseEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    EventDispatcher::Subscribe<MouseScrollEvent>(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    EventDispatcher::Subscribe<WindowResizeEvent>(std::bind(&Application::OnResize, this, std::placeholders::_1));

    float planeVertices[] = {10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,  -10.0f, -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f,
                             -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f, 10.0f,  -0.5f, 10.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
                             -10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f,  10.0f, 10.0f,  -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f};

    uint32_t planeIndices[] = {0, 1, 2, 3, 4, 5};

    auto vertexBuffer = VertexBuffer::Create(planeVertices, sizeof(planeVertices));
    vertexBuffer->SetLayout({{ShaderDataType::Float3, "Position", false}, {ShaderDataType::Float3, "Normal", false}, {ShaderDataType::Float2, "TexCoord", false}});
    auto indexBuffer = IndexBuffer::Create(planeIndices, sizeof(planeIndices));
    auto vertexArray = VertexArray::Create();
    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);

    // Vector3 lightPositions[] = {Vector3(-3.0f, 0.0f, 0.0f), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector3(3.0f, 0.0f, 0.0f)};
    // Vector3 lightColors[] = {Vector3(0.25), Vector3(0.50), Vector3(0.75), Vector3(1.00)};

    auto camera = NewReferencePointer<Camera>(90.0f, 1280.0f, 720.0f);
    camera->Translate(Vector3(0.0f, 3.0f, 12.0f));

    m_Initialized = true;
    OnInit();

    while (!AppWindow->GetWindowClose()) {
        TaskManager::Update();
        OnUpdate();

        // m_WindowRenderer->BeginFrame(camera);
        // m_WindowRenderer->Submit(vertexArray);
        // for (uint32_t i = 0; i < 4; i++) {
        //     m_WindowRenderer->Submit(PointLight(1.0f, 1.0f, 1.0f, lightColors[i], lightColors[i], lightColors[i]), glm::translate(glm::mat4(1.0f), lightPositions[i]));
        // }
        // m_WindowRenderer->EndFrame();

        AppWindow->Render();
        AppWindow->UpdateEvents();
    }

    OnShutdown();
    TaskManager::Shutdown();
    Renderer::SetContext(nullptr);
}

void Application::OnResize(const Event& event) {
    if (!m_Initialized)
        return;
    auto resizeEvent = dynamic_cast<const WindowResizeEvent&>(event);
    HY_LOG_DEBUG("Resizing renderer: {}, {}", resizeEvent.GetWidth(), resizeEvent.GetHeight());
    // m_WindowRenderer->OnResize(resizeEvent.GetWidth(), resizeEvent.GetHeight());
}
