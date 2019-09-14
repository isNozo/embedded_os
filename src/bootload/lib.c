#include "defines.h"
#include "serial.h"
#include "lib.h"

void *memset(void *dst, int c, long len)
{
  char *ret = dst;
  for (; len > 0; len--)
    *(dst++) = c;
  return ret;
}

void *memcpy(void *dst, const void *src, long len)
{
  char *ret = dst;
  for (; len > 0; len--)
    *(dst++) = *(src++);
  return ret;
}

int memcmp(const void *src1, const void *src2, long len)
{
  while (len > 0) {
    if (*src1 != *src2)
      return (*src1 > *src2) ? 1 : -1;
    src1++;
    src2++;
    len--;
  }
  return 0;
}

int strlen(const char *s)
{
  int len = 0;
  for (; *s; s++, len++)
    ;
  return len;
}

char *strcpy(char *dst, const char *src)
{
  char *ret = dst;
  while (!(*src)) {
    *(dst++) = *(src++);
  }
  return ret;
}

int strcmp(const char *src1, const char *src2)
{
  while (*src1 || *src2) {
    if (*src1 != *src2)
      return (*s1 > *s2) ? 1 : -1;
    src1++;
    src2++;
  }
  return 0;
}

int strncmp(const char *src1, const char *src2, int len)
{
  while ((*src1 || *src2) && (len > 0)) {
    if (*src1 != *src2)
      return (*src1 > *src2) ? 1 : -1;
    src1++;
    src2++;
    len--;
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
