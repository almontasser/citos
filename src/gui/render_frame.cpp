#include <gui/render_frame.h>

using namespace citos::common;
using namespace citos::gui;

RenderFrame::RenderFrame(
    common::int32_t w, common::int32_t h)
{
}

RenderFrame::~RenderFrame()
{
}

void RenderFrame::display(GraphicsContext *gc)
{
  for (int ix = 0; ix < 320; ix++)
  {
    for (int iy = 0; iy < 200; iy++)
    {
      gc->PutPixel(ix, iy, pixels[ix][iy].r, pixels[ix][iy].g, pixels[ix][iy].b);
    }
  }
}

void RenderFrame::PutPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b)
{
  if (x >= 0 && x < 320 && y >= 0 && y < 200)
  {
    pixels[x][y].r = r;
    pixels[x][y].g = g;
    pixels[x][y].b = b;
  }
}