/** glfw_window.hh -- Utilities to handle a GLFW window */

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

#ifndef NNM_GLFW_WINDOW_HH_
#define NNM_GLFW_WINDOW_HH_

#include <memory>

#include <GLFW/glfw3.h>

namespace nanamo {

struct WindowDeleter {
    inline void
    operator()(GLFWwindow* ptr)
    {
        glfwDestroyWindow(ptr);
    }
};

using Window = std::unique_ptr<GLFWwindow, WindowDeleter>;

inline Window
createWindow(int width, int height, const char* title, GLFWmonitor* monitor,
             GLFWwindow* share)
{
    auto ptr = glfwCreateWindow(width, height, title, monitor, share);
    if (!ptr) {
        return {};
    }

    return Window(ptr, WindowDeleter{});
}

} // namespace nanamo

#endif /* NNM_GLFW_WINDOW_HH_ */
