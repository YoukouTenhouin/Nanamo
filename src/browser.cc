/** browser.cc -- Browser integration implementation */

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

#include <GL/glew.h>

#include "browser.hh"

namespace nanamo {

BrowserRenderHandler::BrowserRenderHandler(int width, int height)
    : m_width(width), m_height(height)
{
}

void
BrowserRenderHandler::GetViewRect(CefRefPtr<CefBrowser>, CefRect& rect)
{
    rect = CefRect(0, 0, m_width, m_height);
}

void
BrowserRenderHandler::OnPaint(CefRefPtr<CefBrowser>, PaintElementType,
                              const RectList&, const void* data, int width,
                              int height)
{
    if (!data || width <= 0 || height <= 0) {
        /* skip */
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, data);
}

void
BrowserRenderHandler::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

BrowserClient::BrowserClient(CefRefPtr<BrowserRenderHandler> rh)
    : m_renderHandler(rh)
{
}

CefRefPtr<CefRenderHandler>
BrowserClient::GetRenderHandler()
{
    return m_renderHandler;
}

} // namespace nanamo
