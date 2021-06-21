#include <drivers/keyboard.h>

using namespace citos::drivers;

void printf(char *);
void printfHex(uint8_t);

KeyboardEventHandler::KeyboardEventHandler()
{
}

void KeyboardEventHandler::OnKeyDown(char key)
{

}

void KeyboardEventHandler::OnKeyUp(char key)
{
  
}

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler* handler)
    : InterruptHandler(0x21, interruptManager), dataport(0x60), commandport(0x64)
{
  this->handler = handler;
}

KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::Activate()
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

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
  uint8_t key = dataport.Read();
  
  if (handler == 0) return esp;

  static bool shift = false;

  switch (key)
  {
    case 0x01: break; // ESC
    case 0x3B: break; // F1
    case 0x3C: break; // F2
    case 0x3D: break; // F3
    case 0x3E: break; // F4
    case 0x3F: break; // F5
    case 0x40: break; // F6
    case 0x41: break; // F7
    case 0x42: break; // F8
    case 0x43: break; // F9
    case 0x44: break; // F10
    case 0x57: break; // F11
    case 0x58: break; // F12
    case 0x52: break; // INSERT
    case 0x53: break; // DELETE

    case 0x29: if (shift) handler->OnKeyDown('~'); else handler->OnKeyDown('`'); break;
    case 0x02: if (shift) handler->OnKeyDown('!'); else handler->OnKeyDown('1'); break;
    case 0x03: if (shift) handler->OnKeyDown('@'); else handler->OnKeyDown('2'); break;
    case 0x04: if (shift) handler->OnKeyDown('#'); else handler->OnKeyDown('3'); break;
    case 0x05: if (shift) handler->OnKeyDown('$'); else handler->OnKeyDown('4'); break;
    case 0x06: if (shift) handler->OnKeyDown('%'); else handler->OnKeyDown('5'); break;
    case 0x07: if (shift) handler->OnKeyDown('^'); else handler->OnKeyDown('6'); break;
    case 0x08: if (shift) handler->OnKeyDown('&'); else handler->OnKeyDown('7'); break;
    case 0x09: if (shift) handler->OnKeyDown('*'); else handler->OnKeyDown('8'); break;
    case 0x0A: if (shift) handler->OnKeyDown('('); else handler->OnKeyDown('9'); break;
    case 0x0B: if (shift) handler->OnKeyDown(')'); else handler->OnKeyDown('0'); break;
    case 0x0C: if (shift) handler->OnKeyDown('_'); else handler->OnKeyDown('-'); break;
    case 0x0D: if (shift) handler->OnKeyDown('+'); else handler->OnKeyDown('='); break;
    
    case 0x0E: break; // BACKSPACE
    case 0x0F: break; // TAB
    
    case 0x10: if (shift) handler->OnKeyDown('Q'); else handler->OnKeyDown('q'); break;
    case 0x11: if (shift) handler->OnKeyDown('W'); else handler->OnKeyDown('w'); break;
    case 0x12: if (shift) handler->OnKeyDown('E'); else handler->OnKeyDown('e'); break;
    case 0x13: if (shift) handler->OnKeyDown('R'); else handler->OnKeyDown('r'); break;
    case 0x14: if (shift) handler->OnKeyDown('T'); else handler->OnKeyDown('t'); break;
    case 0x15: if (shift) handler->OnKeyDown('Y'); else handler->OnKeyDown('y'); break;
    case 0x16: if (shift) handler->OnKeyDown('U'); else handler->OnKeyDown('u'); break;
    case 0x17: if (shift) handler->OnKeyDown('I'); else handler->OnKeyDown('i'); break;
    case 0x18: if (shift) handler->OnKeyDown('O'); else handler->OnKeyDown('o'); break;
    case 0x19: if (shift) handler->OnKeyDown('P'); else handler->OnKeyDown('p'); break;
    case 0x1A: if (shift) handler->OnKeyDown('{'); else handler->OnKeyDown('['); break;
    case 0x1B: if (shift) handler->OnKeyDown('}'); else handler->OnKeyDown(']'); break;
    case 0x2B: if (shift) handler->OnKeyDown('|'); else handler->OnKeyDown('\\'); break;
    
    case 0x3A: break; // CAPS LOCK
    
    case 0x1E: if (shift) handler->OnKeyDown('A'); else handler->OnKeyDown('a'); break;
    case 0x1F: if (shift) handler->OnKeyDown('S'); else handler->OnKeyDown('s'); break;
    case 0x20: if (shift) handler->OnKeyDown('D'); else handler->OnKeyDown('d'); break;
    case 0x21: if (shift) handler->OnKeyDown('F'); else handler->OnKeyDown('f'); break;
    case 0x22: if (shift) handler->OnKeyDown('G'); else handler->OnKeyDown('g'); break;
    case 0x23: if (shift) handler->OnKeyDown('H'); else handler->OnKeyDown('h'); break;
    case 0x24: if (shift) handler->OnKeyDown('J'); else handler->OnKeyDown('j'); break;
    case 0x25: if (shift) handler->OnKeyDown('K'); else handler->OnKeyDown('k'); break;
    case 0x26: if (shift) handler->OnKeyDown('L'); else handler->OnKeyDown('l'); break;
    case 0x27: if (shift) handler->OnKeyDown(':'); else handler->OnKeyDown(';'); break;
    case 0x28: if (shift) handler->OnKeyDown('\"'); else handler->OnKeyDown('\''); break;
    case 0x1C: handler->OnKeyDown('\n'); break;


    case 0x2C: if (shift) handler->OnKeyDown('Z'); else handler->OnKeyDown('z'); break;
    case 0x2D: if (shift) handler->OnKeyDown('X'); else handler->OnKeyDown('x'); break;
    case 0x2E: if (shift) handler->OnKeyDown('C'); else handler->OnKeyDown('c'); break;
    case 0x2F: if (shift) handler->OnKeyDown('V'); else handler->OnKeyDown('v'); break;
    case 0x30: if (shift) handler->OnKeyDown('B'); else handler->OnKeyDown('b'); break;
    case 0x31: if (shift) handler->OnKeyDown('N'); else handler->OnKeyDown('n'); break;
    case 0x32: if (shift) handler->OnKeyDown('M'); else handler->OnKeyDown('m'); break;
    case 0x33: if (shift) handler->OnKeyDown('<'); else handler->OnKeyDown(','); break;
    case 0x34: if (shift) handler->OnKeyDown('>'); else handler->OnKeyDown('.'); break;
    case 0x35: if (shift) handler->OnKeyDown('?'); else handler->OnKeyDown('/'); break;
    
    case 0x1D: break; // LEFT CTRL
    case 0x5B: break; // WINDOWS
    case 0x38: break; // ALT

    case 0x4B: break; // LEFT ARROW
    case 0x48: break; // UP ARROW
    case 0x4D: break; // RIGHT ARROW
    case 0x50: break; // DOWN ARROW

    case 0x39: handler->OnKeyDown(' '); break;

    case 0x2A: case 0x36: shift = true; break; 
    case 0xAA: case 0xB6: shift = false; break; 

  case 0xFA:
    break;
  case 0x45:
  case 0xC5:
    break;
  default:
    if (key < 0x80)
    {
      printf("KEYBOARD 0x");
      printfHex(key);
    }
    break;
  }

  return esp;
}