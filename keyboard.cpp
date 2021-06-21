#include "keyboard.h"

void printf(char *);

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager)
    : InterruptHandler(0x21, interruptManager), dataport(0x60), commandport(0x64)
{
  // When we start the driver, will remove old key strokes
  while (commandport.Read() & 0x1)
    dataport.Read();
  commandport.Write(0xAE);                        // activate interrupts
  commandport.Write(0x20);                        // command 0x20 = read controller command byte
  uint8_t status = (dataport.Read() | 1) & ~0x10; // sets the first bit, and clear the fifth bit
  commandport.Write(0x60);                        // command 0x60 = set controller command byte
  dataport.Write(status);                         // set state
  dataport.Write(0xF4);                           // activate keyboard
}

KeyboardDriver::~KeyboardDriver()
{
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
  uint8_t key = dataport.Read();
  
  switch (key)
  {
  case 0x01: // ESC
    break;

  case 0x3B: // F1
    break;
  case 0x3C: // F2
    break;
  case 0x3D: // F3
    break;
  case 0x3E: // F4
    break;
  case 0x3F: // F5
    break;
  case 0x40: // F6
    break;
  case 0x41: // F7
    break;
  case 0x42: // F8
    break;
  case 0x43: // F9
    break;
  case 0x44: // F10
    break;
  case 0x57: // F11
    break;
  case 0x58: // F12
    break;
  case 0x52: // INSERT
    break;
  case 0x53: // DELETE
    break;

  case 0x29: printf("`"); break;
  case 0x02: printf("1"); break;
  case 0x03: printf("2"); break;
  case 0x04: printf("3"); break;
  case 0x05: printf("4"); break;
  case 0x06: printf("5"); break;
  case 0x07: printf("6"); break;
  case 0x08: printf("7"); break;
  case 0x09: printf("8"); break;
  case 0x0A: printf("9"); break;
  case 0x0B: printf("0"); break;
  case 0x0C: printf("-"); break;
  case 0x0D: printf("="); break;
  
  case 0x0E: break; // BACKSPACE
  case 0x0F: break; // TAB
  
  case 0x10: printf("q"); break;
  case 0x11: printf("w"); break;
  case 0x12: printf("e"); break;
  case 0x13: printf("r"); break;
  case 0x14: printf("t"); break;
  case 0x15: printf("y"); break;
  case 0x16: printf("u"); break;
  case 0x17: printf("i"); break;
  case 0x18: printf("o"); break;
  case 0x19: printf("p"); break;
  case 0x1A: printf("["); break;
  case 0x1B: printf("]"); break;
  case 0x2B: printf("\\"); break;
  
  case 0x3A: break; // CAPS LOCK
  
  case 0x1E: printf("a"); break;
  case 0x1F: printf("s"); break;
  case 0x20: printf("d"); break;
  case 0x21: printf("f"); break;
  case 0x22: printf("g"); break;
  case 0x23: printf("h"); break;
  case 0x24: printf("j"); break;
  case 0x25: printf("k"); break;
  case 0x26: printf("l"); break;
  case 0x27: printf(";"); break;
  case 0x28: printf("\'"); break;
  case 0x1C: printf("\n"); break; // ENTER


  case 0x2C: printf("z"); break;
  case 0x2D: printf("x"); break;
  case 0x2E: printf("c"); break;
  case 0x2F: printf("v"); break;
  case 0x30: printf("b"); break;
  case 0x31: printf("n"); break;
  case 0x32: printf("m"); break;
  case 0x33: printf(","); break;
  case 0x34: printf("."); break;
  case 0x35: printf("/"); break;
  
  case 0x1D: break; // LEFT CTRL
  case 0x5B: break; // WINDOWS
  case 0x38: break; // ALT

  case 0x4B: break; // LEFT ARROW
  case 0x48: break; // UP ARROW
  case 0x4D: break; // RIGHT ARROW
  case 0x50: break; // DOWN ARROW

  case 0x39: printf(" "); break;

  case 0x2A: case 0x36: break; // SHIFT ON
  case 0xAA: case 0xB6: break; // SHIFT OFF

  case 0xFA:
    break;
  case 0x45:
  case 0xC5:
    break;
  default:
    if (key < 0x80)
    {
      char *msg = "KEYBOARD 0x00 ";
      char *hex = "0123456789ABCDEF";
      msg[11] = hex[(key >> 4) & 0x0F];
      msg[12] = hex[key & 0x0F];
      printf(msg);
    }
    break;
  }

  return esp;
}