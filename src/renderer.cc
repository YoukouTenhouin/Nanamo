/** Renderer.cc -- Renderer implementation */

/*
 * Copyright 2024 Youkou Tenhouin <youkou@tenhou.in>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <mutex>
#include <stdexcept>

#include <glm/ext.hpp>
#include <glm/glm.hpp>

#include "renderer.hh"

namespace nanamo {

static void
errorCallback(int errcode, const char* desc)
{
    static std::mutex lock;
    std::unique_lock guard(lock);

    std::cerr << "GLFW error " << errcode << ": " << desc << std::endl;
}

Renderer::Renderer(const RendererOptions& opts)
{
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        std::abort();
    }
    glfwSetErrorCallback(&errorCallback);

    m_createWindow(opts);
    m_createProgram();
    m_initBuffers();
    m_initTexture();
}

Renderer::~Renderer()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteVertexArrays(1, &m_vertexArray);
    glDeleteProgram(m_program);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void
Renderer::m_createWindow(const RendererOptions& opts)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, opts.border ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, opts.resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   opts.transparent ? GLFW_TRUE : GLFW_FALSE);

    m_window = glfwCreateWindow(640, 480, "Nanamo", nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("GLEW initialization failed");
    }
}

static void
compileShader(GLuint shader, const char* code)
{
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    int infoLogLen = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        std::string msg(infoLogLen + 1, '\0');
        glGetShaderInfoLog(shader, infoLogLen, nullptr, msg.data());
        msg.resize(infoLogLen);
        std::cout << "Shader compilation message: " << msg << std::endl;
    }

    if (result != GL_TRUE) {
        throw std::runtime_error("Failed to compile shader");
    }
}

class ShaderDeleter {
  private:
    GLuint m_shader;

  public:
    ShaderDeleter(GLuint shader) : m_shader(shader) {}
    ~ShaderDeleter()
    {
        glDeleteShader(m_shader);
    }
};

void
Renderer::m_createProgram()
{
    static const char* vertShaderCode =
        "#version 450 core\n"
        "layout(location = 0) in vec2 pos;\n"
        "layout(location = 1) in vec2 uv;\n"
        "out vec2 UV;\n"
        "void main() {\n"
        "UV = uv;\n"
        "gl_Position = vec4(pos, 0.0, 1.0);\n"
        "}";
    static const char* fragShaderCode =
        "#version 450 core\n"
        "in vec2 UV;\n"
        "out vec4 color;\n"
        "uniform sampler2D tex;\n"
        "void main(){\n"
        "color = texture(tex, UV);\n"
        "}";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    ShaderDeleter vertexDeleter(vertexShader);
    ShaderDeleter fragmentDeleter(fragmentShader);

    compileShader(vertexShader, vertShaderCode);
    compileShader(fragmentShader, fragShaderCode);

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    GLint result = GL_FALSE;
    int infoLogLen = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &result);
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLen);

    if (infoLogLen > 0) {
        std::string msg(infoLogLen + 1, '\0');
        glGetProgramInfoLog(m_program, infoLogLen, nullptr, msg.data());
        msg.resize(infoLogLen);
        std::cout << "Program linking message: " << msg << std::endl;
    }

    glDetachShader(m_program, vertexShader);
    glDetachShader(m_program, fragmentShader);

    if (result != GL_TRUE) {
        throw std::runtime_error("Failed to link program");
    }

    m_posLocation = glGetAttribLocation(m_program, "pos");
    m_uvLocation = glGetAttribLocation(m_program, "uv");
    m_texLocation = glGetAttribLocation(m_program, "tex");
}

void
Renderer::m_initBuffers()
{
    glGenVertexArrays(1, &m_vertexArray);
    glBindVertexArray(m_vertexArray);

    static GLfloat vertexBufferData[] = {
        /* clang-format off */
	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f,

	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,
        /* clang-format on */
    };

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData,
                 GL_STATIC_DRAW);

    static GLfloat uvBufferData[] = {
        /* clang-format off */
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
        /* clang-format on */
    };

    glGenBuffers(1, &m_uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufferData), uvBufferData,
                 GL_STATIC_DRAW);
}

void
Renderer::m_initTexture()
{
    static GLubyte values[] = {
        /* clang-format off */
	0, 255, 0, 255,
	255, 255, 255, 127,
	255, 0, 0, 255,
	0, 0, 255, 255,
        /* clang-format on */
    };

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 values);
}

void
Renderer::m_render()
{
    glUseProgram(m_program);

    glEnableVertexAttribArray(m_posLocation);
    glEnableVertexAttribArray(m_uvLocation);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(m_texLocation, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer(m_posLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_uvBuffer);
    glVertexAttribPointer(m_uvLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(m_posLocation);
    glDisableVertexAttribArray(m_uvLocation);
}

void
Renderer::mainLoop()
{
    glClearColor(0.0, 0.0, 0.4, 0.5);
    while (!glfwWindowShouldClose(m_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        m_render();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

} // namespace nanamo
