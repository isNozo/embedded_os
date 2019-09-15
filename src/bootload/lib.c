#include "defines.h"
#include "serial.h"
#include "lib.h"

void *memset(void *dstpp, char c, long len)
{
  char *dstp = dstpp;
  while (len-- > 0)
    *(dstp++) = c;
  return dstpp;
}

void *memcpy(void *dstpp, const void *srcpp, long len)
{
  char *dstp = dstpp;
  const char *srcp = srcpp;
  while (len-- > 0)
    *(dstp++) = *(srcp++);
  return dstpp;
}

int memcmp(const void *srcpp1, const void *srcpp2, long len)
{
  const char *srcp1 = srcp1;
  const char *srcp2 = srcp2;
  while (len-- > 0) {
    if (*srcp1 != *srcp2)
      return (*srcp1 > *srcp2) ? 1 : -1;
    srcp1++;
    srcp2++;
  }
  return 0;
}

int strlen(const char *str)
{
  int len = 0;
  while (*str++)
    len++;
  return len;
}

char *strcpy(char *dst, const char *src)
{
  return memcpy(dst, src, strlen(src) + 1);
}

int strcmp(const char *src1, const char *src2)
{
  while (*src1 || *src2) {
    if (*src1 != *src2)
      return (*src1 > *src2) ? 1 : -1;
    src1++;
    src2++;
  }
  return 0;
}

int strncmp(const char *src1, const char *src2, int len)
{
  while ((*src1 || *src2) && (len-- > 0)) {
    if (*src1 != *src2)
      return (*src1 > *src2) ? 1 : -1;
    src1++;
    src2++;
  }
  return 0;
}

/* １文字送信 */
int putc(unsigned char c)
{
  if (c == '\n')
    serial_send_byte(SERIAL_DEFAULT_DEVICE, '\r');
  return serial_send_byte(SERIAL_DEFAULT_DEVICE, c);
}

/* 文字列送信 */
int puts(unsigned char *str)
{
  while (*str)
    putc(*(str++));
  return 0;
}

int putxval(unsigned long value, int column)
{
  /* 表示バッファ(00000000 - ffffffff) */
  char buf[9];
  char *p;

  /* 表示バッファの終端をポイント */
  p = &buf[9];
  *(p--) = '\0';

  if(!value && !column)
    column++;
  
  while (value || column) {
    *(p--) = "0123456789abcdef"[value & 0xf];
    value >>= 4;
    if (column) column--;
  }

  puts(p + 1);

  return 0;
}
