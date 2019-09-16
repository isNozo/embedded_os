#include "defines.h"
#include "serial.h"
#include "lib.h"

static int init(void)
{
  extern int erodata, data_start, edata, bss_start, ebss;

  memcpy(&data_start, &erodata, (long)&edata - (long)&data_start);
  memset(&bss_start, 0, (long)&ebss - (long)&bss_start);

  serial_init(SERIAL_DEFAULT_DEVICE);
  
  return 0;
}

int main(void)
{
  static char buf[16];
  init();

  puts("\n");
  puts("-----------------------------------\n");
  puts("kzload (kozos boot loader) started.\n");
  puts("-----------------------------------\n");

  while (1) {
    puts("kzload> ");
    gets(buf);

    if (!strcmp(buf, "load")) {
      puts("load started\n");
    } else if (!strcmp(buf, "dump")) {
      puts("dump staretd\n");
    } else {
      puts("unknown command : ");
      puts(buf);
      puts("\n");
    }
  }

  return 0;
}
