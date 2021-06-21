#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

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

  KeyboardEventHandler* handler;

public:
  KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler* handler);
  ~KeyboardDriver();

  virtual void Activate();

  virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif