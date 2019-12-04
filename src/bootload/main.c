#include "defines.h"
#include "xmodem.h"
#include "elf.h"
#include "lib.h"

static int init(void)
{
  extern int erodata, data_start, edata, bss_start, ebss;

  memcpy(&data_start, &erodata, (long)&edata - (long)&data_start);
  memset(&bss_start, 0, (long)&ebss - (long)&bss_start);

  serial_init(SERIAL_DEFAULT_DEVICE);
  
  return 0;
}

static int dump(char *buf, long size)
{
  long i;

  puts("size = ");
  putxval(size, 0);
  puts("\n");

  for (i = 0; i < size; i++) {
    /* show linenum */
    if ((i & 0xf) == 0x0) {
      putxval(i, 4);
      puts(" : ");
    }

    /* show buf */
    putxval(buf[i], 2);

    /* linebreak & sapce */
    if ((i & 0xf) == 0xf) {
      puts("\n");
    } else {
      if ((i & 0xf) == 0x7) puts(" ");
      puts(" ");
    }
  }

  puts("\n");

  return 0;
}

static void wait()
{
  volatile long i;
  for (i = 0; i < 300000; i++)
    ;
}

int main(void)
{
  extern int buffer_start;
  static char buf[16];
  static long size = -1;
  static unsigned char *loadbuf = (char *)(&buffer_start);

  init();

  puts("\n-----------------------------------\n");
  puts("kzload (kozos boot loader) started.\n");
  puts("-----------------------------------\n");

  while (1) {
    puts("kzload> ");
    gets(buf);

    /* load command */
    if (!strcmp(buf, "load")) {
      puts("load started...\n");

      size = xmodem_recv(loadbuf);
      wait();

      if (size < 0) {
	puts("\nXMODEM receive error!\n");
      } else {
	puts("\nXMODEM receive succeeded.\n");
      }

    /* dump command */
    } else if (!strcmp(buf, "dump")) {
      if (size < 0) {
	puts("no data loaded.\n");
      } else {
	dump(loadbuf, size);
      }

    /* run command */
    } else if (!strcmp(buf, "run")) {
      if (size < 0) {
	puts("no data loaded.\n");
      } else {
	elf_load(loadbuf);
      }

    /* unkown command */
    } else {
      puts("unknown command : ");
      puts(buf);
      puts("\n");
    }
  }

  return 0;
}
