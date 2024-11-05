/** main.cc -- Program entry point */

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
#include <memory>
#include <mutex>

#include <getopt.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "glfw_window.hh"

static bool ARG_border = false;
static bool ARG_resizable = false;
static bool ARG_transparent = false;

static const char* cmdName = "nanamo";

static void
usage(std::ostream& os = std::cout)
{
    os << "usage: " << cmdName << " [options]" << std::endl;
    os << "  options:" << std::endl;
    os << "    -b, --border\t"
       << "Enable window border" << std::endl;
    os << "    -h, --help\t\t"
       << "Show this help message" << std::endl;
    os << "    -r, --resizable\t"
       << "Make window resizable" << std::endl;
    os << "    -t, --transparent\t"
       << "Enable transparent background" << std::endl;
}

static void
parseArgs(int argc, char** argv)
{
    static struct option longOpts[] = {
        {"border", 0, nullptr, 'b'},
        {"help", 0, nullptr, 'h'},
        {"resizable", 0, nullptr, 'r'},
        {"transparent", 0, nullptr, 't'},
    };

    bool running = true;
    while (running) {
        int c = getopt_long(argc, argv, "bhrt", longOpts, nullptr);
        switch (c) {
        case -1:
            running = false;
            break;
        case 'b':
            ARG_border = true;
            break;
        case 'h':
            usage();
            std::exit(0);
	case 'r':
	    ARG_resizable = true;
	    break;
        case 't':
            ARG_transparent = true;
            break;
        case '?':
            usage(std::cerr);
            std::exit(-1);
        default:
            std::cerr << "getopt_long returned " << c << std::endl;
            std::abort();
        }
    }
}

static void
errorCallback(int errcode, const char* desc)
{
    static std::mutex lock;
    std::unique_lock guard(lock);

    std::cerr << "GLFW error " << errcode << ": " << desc << std::endl;
}

void
mainLoop()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, ARG_border ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, ARG_resizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   ARG_transparent ? GLFW_TRUE : GLFW_FALSE);

    nanamo::Window window =
        nanamo::createWindow(640, 480, "Nanamo", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("failed to create GLFW window");
    }

    glfwMakeContextCurrent(window.get());

    while (!glfwWindowShouldClose(window.get())) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }
}

int
main(int argc, char* argv[])
{
    parseArgs(argc, argv);

    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed" << std::endl;
        std::abort();
    }
    glfwSetErrorCallback(&errorCallback);

    try {
        mainLoop();
    } catch (const std::exception& e) {
        std::cerr << "Exception thrown from main loop: " << e.what()
                  << std::endl;
    }

    glfwTerminate();
}
