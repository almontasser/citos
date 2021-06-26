#ifndef __CITOS__GUI__RENDER_H
#define __CITOS__GUI__RENDER_H

#include <drivers/vga.h>
#include <common/graphics_context.h>

namespace citos
{
  namespace gui
  {

    using namespace citos::common;

    class Pixel
    {
    public:
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };

    class RenderFrame : public VideoGraphicsArray
    {
    private:
      Pixel pixels[320][200];

    public:
      RenderFrame(
          int32_t w, int32_t h);

      ~RenderFrame();

      void display(GraphicsContext *gc);

      void PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
    };
  }
}

#endif