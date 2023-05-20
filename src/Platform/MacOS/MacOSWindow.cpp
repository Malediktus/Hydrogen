#include <Hydrogen/Platform/MacOS/MacOSWindow.hpp>
#include <Hydrogen/Core/Logger.hpp>

using namespace Hydrogen;

MacOSWindow::MacOSWindow(const std::string& title, uint32_t width, uint32_t height) {
    HY_ASSERT(glfwInit(), "Init glfw");

    auto api = RenderWindow::ChooseRenderingAPI();

    glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
    if (api == RendererAPI::API::OpenGL) {
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    } else {
        HY_INVOKE_ERROR("Only OpenGL is supported for now");
    }

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    HY_ASSERT(m_Window, "glfw window is null");
}

MacOSWindow::~MacOSWindow() {
    glfwTerminate();
}

void MacOSWindow::SetTitle(const std::string& title) {
    glfwSetWindowTitle(m_Window, title.c_str());
}

uint32_t MacOSWindow::GetWidth() const {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return (uint32_t) width;
}

uint32_t MacOSWindow::GetHeight() const {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return (uint32_t) height;
}

glm::vec2 MacOSWindow::GetViewportSize() const {
    int width, height;
    glfwGetWindowSize(m_Window, &width, &height);
    return glm::vec2(width, height);
}

bool MacOSWindow::GetWindowClose() const {
    return glfwWindowShouldClose(m_Window);
}

bool MacOSWindow::GetKeyDown(KeyCode key) const {
    return glfwGetKey(m_Window, (int) key) == GLFW_PRESS;
}

bool MacOSWindow::GetKey(KeyCode key) const {
    return glfwGetKey(m_Window, (int) key) == GLFW_PRESS;
}

bool MacOSWindow::GetKeyUp(KeyCode key) const {
    return glfwGetKey(m_Window, (int) key) == GLFW_RELEASE;
}

bool MacOSWindow::GetMouseKeyDown(KeyCode key) const {
    return glfwGetMouseButton(m_Window, (int) key) == GLFW_PRESS;
}

bool MacOSWindow::GetMouseKey(KeyCode key) const {
    return glfwGetMouseButton(m_Window, (int) key) == GLFW_PRESS;
}

bool MacOSWindow::GetMouseKeyUp(KeyCode key) const {
    return glfwGetMouseButton(m_Window, (int) key) == GLFW_RELEASE;
}

uint32_t MacOSWindow::GetMouseX() const {
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return x;
}

uint32_t MacOSWindow::GetMouseY() const {
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return y;
}

glm::vec2 MacOSWindow::GetMousePos() const {
    double x, y;
    glfwGetCursorPos(m_Window, &x, &y);
    return glm::vec2(x, y);
}

void MacOSWindow::UpdateEvents() {
    glfwPollEvents();
}

void MacOSWindow::Render() {
    glfwSwapBuffers(m_Window);

    // Only for MacOS (Because of some strange glfw bug)
    static bool moved = false;
    if (!moved) {
        int x, y;
        glfwGetWindowPos(m_Window, &x, &y);
        glfwSetWindowPos(m_Window, x + 1, y);
        glfwSetWindowPos(m_Window, x, y);
        moved = true;
    }
}

void MacOSWindow::SetupOpenglContext(int, int) {
    glfwMakeContextCurrent(m_Window);
}

void* MacOSWindow::GetWindowOpenGLProcAddress() {
    return (void*) glfwGetProcAddress;
}
