#include <drivers/mouse.h>

using namespace citos::drivers;

void printf(char *);

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::OnActivate()
{
}


void MouseEventHandler::OnMouseDown(uint8_t button)
{
}

void MouseEventHandler::OnMouseUp(uint8_t button)
{
}

void MouseEventHandler::OnMouseMove(int xoffset, int yoffset)
{
}


MouseDriver::MouseDriver(InterruptManager *interruptManager, MouseEventHandler* handler)
    : InterruptHandler(0x2C, interruptManager), dataport(0x60), commandport(0x64)
{
  this->handler = handler;
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate()
{
  offset = 0;
  buttons = 0;

  if (handler != 0)
    handler->OnActivate();

  commandport.Write(0xA8);              // activate interrupts
  commandport.Write(0x20);              // command 0x20 = read controller command byte
  uint8_t status = dataport.Read() | 2; // sets the second bit
  commandport.Write(0x60);              // command 0x60 = set controller command byte
  dataport.Write(status);               // set state
  commandport.Write(0xD4);
  dataport.Write(0xF4);
  dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
  // check if there is data to read
  uint8_t status = commandport.Read();
  if (!(status & 0x20))
    return esp;

  buffer[offset] = dataport.Read();

  if (handler == 0)
    return esp;

  offset = (offset + 1) % 3;

  if (offset == 0)
  {
    if (buffer[1] != 0 || buffer[2] != 0) {
      handler->OnMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));
    }
  }

  for (uint8_t i = 0; i < 3; i++)
  {
    if ((buffer[0] & (1 << i)) != (buttons & (1 << i)))
    {
      if (buttons & (1 << i))
        handler->OnMouseUp(i+1);
      else
        handler->OnMouseDown(i+1);
    }
  }
  buttons = buffer[0];

  return esp;
}