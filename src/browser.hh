/** browser.hh -- Browser integration definition */

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

#ifndef NNM_RENDER_HANDLER_HH_
#define NNM_RENDER_HANDLER_HH_

#include <cef_client.h>
#include <cef_render_handler.h>

namespace nanamo {

class BrowserRenderHandler : public CefRenderHandler {
  private:
    int m_width = 0;
    int m_height = 0;

  public:
    BrowserRenderHandler(int width, int height);

    void GetViewRect(CefRefPtr<CefBrowser>, CefRect&) override final;
    void OnPaint(CefRefPtr<CefBrowser>, PaintElementType, const RectList&,
                 const void*, int width, int height) override final;

    void resize(int width, int height);

    IMPLEMENT_REFCOUNTING(BrowserRenderHandler);
};

class BrowserClient : public CefClient {
  private:
    CefRefPtr<BrowserRenderHandler> m_renderHandler;

  public:
    BrowserClient(CefRefPtr<BrowserRenderHandler>);

    CefRefPtr<CefRenderHandler> GetRenderHandler() override final;

    IMPLEMENT_REFCOUNTING(BrowserClient);
};

} // namespace nanamo

#endif /* NNM_RENDER_HANDLER_HH_ */
