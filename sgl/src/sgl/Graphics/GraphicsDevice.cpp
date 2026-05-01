#include "GraphicsDevice.h"
#include <glad/glad.h>
#include <sgl/Util/Logger.h>
#include <sgl/Game.h>

static void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	sgl::String msg;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		msg += u8"Source: API\n";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		msg += u8"Source: Window System\n";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		msg += u8"Source: Shader Compiler\n";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		msg += u8"Source: Third Party\n";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		msg += u8"Source: Application\n";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		msg += u8"Source: Other\n";
		break;
	default:
		msg += u8"Source: Unknown\n";
		break;
	};

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		msg += u8"Type: Error\n";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		msg += u8"Type: Deprecated Behaviour\n";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		msg += u8"Type: Undefined Behaviour\n";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		msg += u8"Type: Portability\n";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		msg += u8"Type: Performance\n";
		break;
	case GL_DEBUG_TYPE_MARKER:
		msg += u8"Type: Marker\n";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		msg += u8"Type: Push Group\n";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		msg += u8"Type: Pop Group\n";
		break;
	case GL_DEBUG_TYPE_OTHER:
		msg += u8"Type: Other\n";
		break;
	default:
		msg += u8"Type: Unknown\n";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		msg += u8"Severity: High\n";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		msg += u8"Severity: Medium\n";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		msg += u8"Severity: Low\n";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		msg += u8"Severity: Notification\n";
		break;
	default:
		msg += u8"Severity: Unknown\n";
		break;
	}

	msg += sgl::ConvertString<const char*, sgl::String>(message);
	sgl::Logger::Log(msg);
}

sgl::ClearFlag sgl::operator|(ClearFlag a, ClearFlag b) {
    return (ClearFlag)((uint8)a | (uint8)b);
}

sgl::ClearFlag sgl::operator&(ClearFlag a, ClearFlag b) {
    return (ClearFlag)((uint8)a & (uint8)b);
}

sgl::ClearFlag& sgl::operator|=(ClearFlag& a, ClearFlag b)
{
    a = (ClearFlag)((uint8)a | (uint8)b);
    return a;
}

sgl::ClearFlag& sgl::operator&=(ClearFlag& a, ClearFlag b)
{
    a = (ClearFlag)((uint8)a & (uint8)b);
    return a;
}

void sgl::GraphicsDevice::SetClearColour(const Colour& colour)
{
    mClearColour = colour;
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void sgl::GraphicsDevice::Resize(const Vec2i &newsize)
{
    mWidth = newsize.width;
    mHeight = newsize.height;

    glViewport(0, 0, mWidth, mHeight);
}

void sgl::GraphicsDevice::Clear(ClearFlag flag)
{
    GLbitfield glFlags = 0;
    if ((uint8)(flag & ClearFlag::COLOUR) != 0)
        glFlags |= GL_COLOR_BUFFER_BIT;

    if ((uint8)(flag & ClearFlag::DEPTH) != 0)
        glFlags |= GL_DEPTH_BUFFER_BIT;

    glClear(glFlags);
}


void sgl::GraphicsDevice::Present()
{
    SDL_GL_SwapWindow(mWindow);
}

void sgl::GraphicsDevice::Init()
{
    glEnable(GL_DEPTH_TEST);

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLDebugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

    mWindow = Get<Game>().GetWindow().SDLWindow();

    const Vec2i& size = Get<Game>().GetWindow().ScreenSize();
    mWidth = size.width;
    mHeight = size.height;

    glViewport(0, 0, mWidth, mHeight);

    Logger::Log(u8"SDL Version: " + AnyToString(SDL_VERSIONNUM_MAJOR(SDL_VERSION)) + u8"." + AnyToString(SDL_VERSIONNUM_MINOR(SDL_VERSION)) + u8"." + AnyToString(SDL_VERSIONNUM_MICRO(SDL_VERSION)));
    Logger::Log(String(u8"OpenGL Version: ") + (char8_t*)glGetString(GL_VERSION));
    Logger::Log(String(u8"GLSL Version: ") + (char8_t*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    Logger::Log(String(u8"Vendor: ") + (char8_t*)glGetString(GL_VENDOR));
    Logger::Log(String(u8"Renderer: ") + (char8_t*)glGetString(GL_RENDERER));
}

void sgl::GraphicsDevice::Shutdown()
{
}