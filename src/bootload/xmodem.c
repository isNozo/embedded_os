#include "defines.h"
#include "serial.h"
#include "lib.h"
#include "xmodem.h"

#define XMODEM_SOH 0x01
#define XMODEM_STX 0x02
#define XMODEM_EOT 0x04
#define XMODEM_ACK 0x06
#define XMODEM_NAK 0x15
#define XMODEM_CAN 0x18
#define XMODEM_EOF 0x1a /* Ctrl-Z */

#define XMODEM_BLOCK_SIZE 128

/* 受信可能になるまで定期的にNAKを送信する */
static int xmodem_wait(void)
{
  long cnt = 0;
    
  while (!serial_is_recv_enable(SERIAL_DEFAULT_DEVICE)) {
    if (++cnt >= 2000000) {
      cnt = 0;
      serial_send_byte(SERIAL_DEFAULT_DEVICE, XMODEM_NAK);
    }
  }

  return 0;
}

/* 
 * １ブロックの受信
 *     1 byte : ブロック番号
 *     1 byte : ブロック番号の反転
 *   128 byte : データ
 *     1 byte : チェックサム
 */
static int xmodem_read_block(unsigned char block_number, char *buf)
{
  unsigned char c, block_num, check_sum;
  int i;
  
  /* ブロック番号 */
  block_num = serial_recv_byte(SERIAL_DEFAULT_DEVICE);
  if (block_num != block_number)
    return -1;

  /* ブロック番号の反転 */
  block_num ^= serial_recv_byte(SERIAL_DEFAULT_DEVICE);
  if (block_num != 0xff)
    return -1;

  /* データ */
  check_sum = 0;
  for (i = 0; i < XMODEM_BLOCK_SIZE; i++) {
    c = serial_recv_byte(SERIAL_DEFAULT_DEVICE);
    *(buf++) = c;
    check_sum += c;
  }

  /* チェックサム */
  check_sum ^= serial_recv_byte(SERIAL_DEFAULT_DEVICE);
  if (check_sum)
    return -1;

  return i;
}

long xmodem_recv(char *buf)
{
  int res, receiving = 0;
  long size = 0;
  unsigned char block_number = 1;

  while (1) {
    if (!receiving)
      xmodem_wait();	/* 受信可能になるまで定期的にNAKを送信する */
    
    switch (serial_recv_byte(SERIAL_DEFAULT_DEVICE)) {
    case XMODEM_EOT:	/* 受信終了 */
      serial_send_byte(SERIAL_DEFAULT_DEVICE, XMODEM_ACK);
      return size;

    case XMODEM_CAN:	/* 受信中断 */
      return -1;

    case XMODEM_SOH:	/* 受信開始 */
      receiving = 1;
      res = xmodem_read_block(block_number, buf);

      if(res < 0) {	/* 受信エラー */
	serial_send_byte(SERIAL_DEFAULT_DEVICE, XMODEM_NAK);
      } else {		/* 受信成功 */
	block_number++;
	size += res;
	buf  += res;
	serial_send_byte(SERIAL_DEFAULT_DEVICE, XMODEM_ACK);
      }
      break;
      
    default:
      if (receiving)
	return -1;
    }
  }
}
