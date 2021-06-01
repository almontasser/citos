typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
  {
    (*i)();
  }
}

void clear_screen()
{
  unsigned short *video_memory = (unsigned short *)0xB8000;
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
  unsigned short *video_memory = (unsigned short *)0xB8000;

  for (int i = 0; str[i] != '\0'; i++)
  {
    video_memory[i] = (video_memory[i] & 0xFF00) | str[i];
  }
}

extern "C" void kernel_main(void *multiboot_structure, unsigned int magic_number)
{
  clear_screen();
  printf("CIT Operating System\0");

  while (1)
  {
  };
}
