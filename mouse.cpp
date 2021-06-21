#include "mouse.h"

void printf(char *);

MouseDriver::MouseDriver(InterruptManager *interruptManager)
    : InterruptHandler(0x2C, interruptManager), dataport(0x60), commandport(0x64)
{
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate()
{
  offset = 0;
  buttons = 0;

  uint16_t *video_memory = (uint16_t *)0xB8000;

  video_memory[80 * 12 + 40] = ((video_memory[80 * 12 + 40] & 0xF000) >> 4) |
                               ((video_memory[80 * 12 + 40] & 0x0F00) << 4) |
                               (video_memory[80 * 12 + 40] & 0x00FF);

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
  static uint16_t *video_memory = (uint16_t *)0xB8000;

  // check if there is data to read
  uint8_t status = commandport.Read();
  if (!(status & 0x20))
    return esp;

  static int8_t x = 40, y = 12;

  buffer[offset] = dataport.Read();
  offset = (offset + 1) % 3;

  if (offset == 0)
  {
    video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                               ((video_memory[80 * y + x] & 0x0F00) << 4) |
                               (video_memory[80 * y + x] & 0x00FF);

    x += buffer[1];
    if (x < 0) x = 0;
    if (x >= 80) x = 79;
    y -= buffer[2];
    if (y < 0) y = 0;
    if (y >= 25) y = 24;

    video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                               ((video_memory[80 * y + x] & 0x0F00) << 4) |
                               (video_memory[80 * y + x] & 0x00FF);
  }

  for (uint8_t i = 0; i < 3; i++)
  {
    if ((buffer[0] & (1 << i)) != (buttons & (1 << i)))
    {
      // button click
      video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                                 ((video_memory[80 * y + x] & 0x0F00) << 4) |
                                 (video_memory[80 * y + x] & 0x00FF);
    }
  }
  buttons = buffer[0];

  return esp;
}