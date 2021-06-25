#ifndef __CITOS_DESKTOP_H
#define __CITOS_DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace citos
{
  namespace gui
  {

    using namespace citos::drivers;
    
    class Desktop : public CompositeWidget, public MouseEventHandler
    {
    protected:
      uint32_t MouseX;
      uint32_t MouseY;
    
    public:
      Desktop(uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b);
      ~Desktop();

      void Draw(GraphicsContext* gc);

      virtual void OnMouseDown(uint8_t button);
      virtual void OnMouseUp(uint8_t button);
      virtual void OnMouseMove(int xoffset, int yoffset);
    };

  } // namespace gui
  
} // namespace citos


#endif