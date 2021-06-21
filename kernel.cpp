#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "driver.h"
#include "keyboard.h"
#include "mouse.h"

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

extern "C" void kernel_main(void *multiboot_structure, unsigned int magic_number)
{
  clear_screen();
  printf("CIT Operating System\n");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(&gdt);

  printf("Initializin Hardware, Stage 1\n");
  DriverManager driverManager;

  KeyboardDriver keyboard(&interrupts);
  driverManager.AddDriver(&keyboard);

  MouseDriver mouse(&interrupts);
  driverManager.AddDriver(&mouse);

  printf("Initializin Hardware, Stage 2\n");
  driverManager.ActivateAll();

  printf("Initializin Hardware, Stage 3\n");
  interrupts.Activate();

  while (1)
  {
  };
}
