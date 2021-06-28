#include <common/types.h>
#include <gdt.h>
#include <hardware/interrupts.h>
#include <hardware/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/window.h>
#include <gui/desktop.h>
#include <gui/render_frame.h>

using namespace citos;
using namespace citos::common;
using namespace citos::hardware;
using namespace citos::drivers;
using namespace citos::gui;

#define GRAPHICSMODE

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
  {
    (*i)();
  }
}

void clear_screen()
{
  static uint16_t *video_memory = (uint16_t *)0xB8000;

  for (int y = 0; y < 25; y++)
  {
    for (int x = 0; x < 80; x++)
    {
      *video_memory = (*video_memory & 0xFF00) | ' ';
      video_memory++;
    }
  }
}

void printf(char *str)
{
  static uint16_t *video_memory = (uint16_t *)0xB8000;

  static uint8_t x = 0, y = 0;

  for (int i = 0; str[i] != '\0'; i++)
  {
    if (x >= 80)
    {
      x = 0;
      y++;
    }

    if (y >= 25)
    {
      x = 0;
      y = 0;
      clear_screen();
    }

    switch (str[i])
    {
    case '\n':
      x = 0;
      y++;
      break;
    default:
      video_memory[y * 80 + x] = (video_memory[i] & 0xFF00) | str[i];
      x++;
    }
  }
}

void printfHex(uint8_t byte)
{
  char *msg = "00";
  char *hex = "0123456789ABCDEF";
  msg[0] = hex[(byte >> 4) & 0x0F];
  msg[1] = hex[byte & 0x0F];
  printf(msg);
}

class PrintKeyboardEventHandler : public KeyboardEventHandler
{
public:
  virtual void OnKeyDown(char key)
  {
    char* c = " ";
    c[0] = key;
    printf(c);
  }
};

class MouseToConsole : public MouseEventHandler
{
public:
  int8_t x, y;

  virtual void OnActivate()
  {
    static uint16_t *video_memory = (uint16_t *)0xB8000;

    x = 40;
    y = 12;

    video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                                 ((video_memory[80 * y + x] & 0x0F00) << 4) |
                                 (video_memory[80 * y + x] & 0x00FF);
  }

  virtual void OnMouseMove(int xoffset, int yoffset)
  {
    static uint16_t *video_memory = (uint16_t *)0xB8000;

    video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                               ((video_memory[80 * y + x] & 0x0F00) << 4) |
                               (video_memory[80 * y + x] & 0x00FF);

    x += xoffset;
    if (x < 0) x = 0;
    if (x >= 80) x = 79;
    y += yoffset;
    if (y < 0) y = 0;
    if (y >= 25) y = 24;

    video_memory[80 * y + x] = ((video_memory[80 * y + x] & 0xF000) >> 4) |
                               ((video_memory[80 * y + x] & 0x0F00) << 4) |
                               (video_memory[80 * y + x] & 0x00FF);
  }
};

extern "C" void kernel_main(void *multiboot_structure, unsigned int magic_number)
{
  clear_screen();
  printf("CIT Operating System\n");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);

  printf("Initializin Hardware, Stage 1\n");

#ifdef GRAPHICSMODE
  Desktop desktop(320,200, 0x00,0x00,0xA8);
#endif

  DriverManager driverManager;

#ifdef GRAPHICSMODE
    KeyboardDriver keyboard(&interrupts, &desktop);
#else
  PrintKeyboardEventHandler keyboardHandler;
  KeyboardDriver keyboard(&interrupts, &keyboardHandler);
#endif
  driverManager.AddDriver(&keyboard);

#ifdef GRAPHICSMODE
  MouseDriver mouse(&interrupts, &desktop);
#else
  MouseToConsole mouseEventHandler;
  MouseDriver mouse(&interrupts, &mouseEventHandler);
#endif
  driverManager.AddDriver(&mouse);

  PeripheralComponentInterconnectController PCIController;
  PCIController.SelectDrivers(&driverManager, &interrupts);
#ifdef GRAPHICSMODE
  VideoGraphicsArray vga;
  //initialize render frame
  RenderFrame renderFrame(320,200); 
#endif


  printf("Initializin Hardware, Stage 2\n");
  driverManager.ActivateAll();

  printf("Initializin Hardware, Stage 3\n");
  interrupts.Activate();

#ifdef GRAPHICSMODE
  vga.SetMode(320, 200, 8);
#endif

 #ifdef GRAPHICSMODE
  vga.SetMode(320,200,8);
  Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
  Widget w(&win1, 4, 4, 4, 4, 0xFF, 0xFF, 0xFF);
  win1.AddChild(&w);
  desktop.AddChild(&win1);
  Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
  desktop.AddChild(&win2);
#endif

  while (1)
  {
    #ifdef GRAPHICSMODE
      // desktop.Draw(&vga);
      desktop.Draw(&renderFrame);
      renderFrame.display(&vga);
    #endif
  };
}
