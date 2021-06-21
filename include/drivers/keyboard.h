#ifndef __CITOS_DRIVERS_KEYBOARD_H
#define __CITOS_DRIVERS_KEYBOARD_H

#include <common/types.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <hardware/port.h>

namespace citos
{
  namespace drivers
  {

    using namespace citos::common;
    using namespace citos::hardware;

    class KeyboardEventHandler
    {
    public:
      KeyboardEventHandler();

      virtual void OnKeyDown(char key);
      virtual void OnKeyUp(char key);
    };

    class KeyboardDriver : public InterruptHandler, public Driver
    {
    protected:
      Port8Bit dataport;
      Port8Bit commandport;

      KeyboardEventHandler *handler;

    public:
      KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler *handler);
      ~KeyboardDriver();

      virtual void Activate();

      virtual uint32_t HandleInterrupt(uint32_t esp);
    };

  } // namespace drivers

} // namespace citos

#endif