#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>
#include <stdio.h>

void uart_init (void);
void uart_send_data (uint16_t data);
int fputc (int ch, FILE *f);

#endif // SERIAL_H_
