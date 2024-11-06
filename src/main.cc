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
#include <string>
#include <string_view>

#include <getopt.h>

#include <cef_app.h>

#include "renderer.hh"

static bool ARG_border = false;
static bool ARG_resizable = false;
static bool ARG_transparent = false;
static std::string ARG_url = "";

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

    if (argc - optind != 1) {
        if (optind >= argc) {
            std::cerr << "error: not enough arguments" << std::endl;
        } else {
            std::cerr << "error: too many arugments" << std::endl;
        }
        usage(std::cerr);
        std::exit(-1);
    }
    ARG_url = argv[optind];
}

static void
startCEF(int argc, char* argv[])
{
    CefMainArgs args(argc, argv);
    auto result = CefExecuteProcess(args, nullptr, nullptr);
    if (result == -1) {
        /* Called for the browser process. */
    } else if (result >= 0) {
        /* Forked process has ended. */
        std::exit(result);
    }

    CefSettings settings;
    settings.windowless_rendering_enabled = true;
    CefString(&settings.cache_path) = "/tmp/nanamo-cache";

    CefInitialize(args, settings, nullptr, nullptr);
}

int
main(int argc, char* argv[])
{
    if (std::string_view(argv[1]).starts_with("--type")) {
        /* This is a CEF helper process, skip argument parsing */
    } else {
        parseArgs(argc, argv);
    }

    startCEF(argc, argv);

    nanamo::RendererOptions options = {
        .border = ARG_border,
        .resizable = ARG_resizable,
        .transparent = ARG_transparent,
        .url = ARG_url,
    };

    nanamo::Renderer renderer(options);

    try {
        renderer.mainLoop();
    } catch (const std::exception& e) {
        std::cerr << "Exception thrown from main loop: " << e.what()
                  << std::endl;
    }

    CefShutdown();
}
