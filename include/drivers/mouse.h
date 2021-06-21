#ifndef __CITOS_DRIVERS_MOUSE_H
#define __CITOS_DRIVERS_MOUSE_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <hardware/port.h>
#include <drivers/driver.h>

namespace citos
{
  namespace drivers
  {

    using namespace citos::common;
    using namespace citos::hardware;

    class MouseEventHandler
    {
    public:
      MouseEventHandler();

      virtual void OnActivate();
      virtual void OnMouseDown(uint8_t button);
      virtual void OnMouseUp(uint8_t button);
      virtual void OnMouseMove(int xoffset, int yoffset);
    };

    class MouseDriver : public InterruptHandler, public Driver
    {
    protected:
      Port8Bit dataport;
      Port8Bit commandport;

      uint8_t buffer[3];
      uint8_t offset;
      uint8_t buttons;

      MouseEventHandler *handler;

    public:
      MouseDriver(InterruptManager *interruptManager, MouseEventHandler *handler);
      ~MouseDriver();

      virtual void Activate();

      virtual uint32_t HandleInterrupt(uint32_t esp);
    };

  } // namespace drivers

} // namespace citos

#endif