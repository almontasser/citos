#include "keyboard.h"

void printf(char *);

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager)
    : InterruptHandler(0x21, interruptManager), dataport(0x60), commandport(0x64)
{
  // When we start the driver, will remove old key strokes
  while (commandport.Read() & 0x1)
    dataport.Read();
  commandport.Write(0xAE);                        // activate interrupts
  commandport.Write(0x20);                        // get current state
  uint8_t status = (dataport.Read() | 1) & ~0x10; // sets the first bit, and clear the fifth bit
  commandport.Write(0x60);                        // set state
  dataport.Write(status);                         // set state
  dataport.Write(0xF4);                           // activate keyboard
}

KeyboardDriver::~KeyboardDriver()
{
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
  uint8_t key = dataport.Read();

  char *msg = "KEYBOARD 0x00 ";
  char *hex = "0123456789ABCDEF";
  msg[11] = hex[(key >> 4) & 0x0F];
  msg[12] = hex[key & 0x0F];
  printf(msg);

  return esp;
}