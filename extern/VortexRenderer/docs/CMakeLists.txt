set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN NO)
set(DOXYGEN_GENERATE_RTF NO)
set(DOXYGEN_PROJECT_NAME "Vortex")
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(GENERATE_TREEVIEW YES)
set(DISABLE_INDEX NO)
set(FULL_SIDEBAR NO)
set(HTML_COLORSTYLE LIGHT)

doxygen_add_docs(docs
    Vortex/Vortex.hpp
    Vortex/VertexArray.hpp
    Vortex/Texture.hpp
    Vortex/Shader.hpp
    Vortex/RendererAPI.hpp
    Vortex/Renderer.hpp
    Vortex/RenderCommand.hpp
    Vortex/Core.hpp
    Vortex/Context.hpp
    Vortex/Camera.hpp
    Vortex/Buffer.hpp
    "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/shader.md"
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include")
