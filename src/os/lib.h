#ifndef _LIB_H_INCLUDED_
#define _LIB_H_INCLUDED_

#ifdef DEBUG
#define puts_d(str) puts(str)
#define putxval_d(value, column) putxval(value, column)
#else
#define puts_d(str)
#define putxval_d(value, column)
#endif

void *memset(void *dstpp, char c, long len);
void *memcpy(void *dstpp, const void *srcpp, long len);
int memcmp(const void *srcpp1, const void *srcpp2, long len);
int strlen(const char *str);
char *strcpy(char *dst, const char *src);
int strcmp(const char *src1, const char *src2);
int strncmp(const char *src1, const char *src2, int len);

int putc(unsigned char c);    /* １文字送信 */
unsigned char getc(void);     /* １文字受信 */
int puts(unsigned char *str); /* 文字列送信 */
int gets(unsigned char *buf); /* 文字列受信 */

int putxval(unsigned long value, int column);

#endif
