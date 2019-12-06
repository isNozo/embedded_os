#include "defines.h"
#include "serial.h"
#include "lib.h"

int main(void)
{
  static char buf[32];
  
  puts("Hello World!\n");

  while(1) {
    puts("> ");
    gets(buf);

    /* echo command */
    if (!strncmp(buf, "echo ", 5)) {
      puts(buf + 5);
      puts("\n");
    /* exit command */
    } else if (!strcmp(buf, "exit")) {
      break;
    /* unkown command */
    } else {
      puts("unknown command : ");
      puts(buf);
      puts("\n");
    }
  }

  return 0;
}
