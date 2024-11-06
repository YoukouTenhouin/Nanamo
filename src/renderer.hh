/** renderer.hh -- Renderer definitions */

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

#ifndef NNM_RENDERER_HH_
#define NNM_RENDERER_HH_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace nanamo {

struct RendererOptions {
    bool border = false;
    bool resizable = false;
    bool transparent = false;
};

class Renderer {
  private:
    GLFWwindow *m_window = nullptr;

    GLuint m_program;
    GLuint m_vertexArray;
    GLuint m_vertexBuffer;
    GLuint m_uvBuffer;
    GLuint m_texture;
    GLuint m_posLocation;
    GLuint m_uvLocation;
    GLuint m_texLocation;

    void m_createWindow(const RendererOptions&);
    void m_createProgram();
    void m_initBuffers();
    void m_initTexture();

    void m_render();

  public:
    Renderer(const RendererOptions&);
    ~Renderer();
    void mainLoop();
};

} // namespace nanamo

#endif /* NNM_RENDERER_HH_ */
