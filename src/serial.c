#include <stdint.h>
#include <stdio.h>

#include "../inc/tm4c123gh6pm.h"

void
Uart_Init (void) // UART0 -> PA0(RX) PA1(TX) 9600
{
  SYSCTL_RCGCUART_R |= (1 << 0); // enable the uart0 clock
  SYSCTL_RCGCGPIO_R |= (1 << 0); // enable the GPIOA clock

  GPIO_PORTA_AFSEL_R
      |= (1 << 0)
         | (1 << 1); // set alternative function of PA[0, 1] as uart0 pins
  GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4); // set uart mode
  GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);  // digitize the GPIO pins of uart0

  UART0_CTL_R &= ((unsigned)(~(1 << 0))) & ((unsigned)(~(1 << 8)))
                 & ((unsigned)(~(1 << 9))); // disable uart0, TX and RX
  UART0_IBRD_R = 104;                       // set baud rate in IBRD
  UART0_FBRD_R = 11;                        // set baud rate in FBRD
  UART0_LCRH_R |= (3 << 5); // set data length in the transmission
  UART0_CC_R = 0x05;        // select clock source
  UART0_CTL_R |= (1 << 0) | (1 << 8) | (1 << 9); // enable uart0, TX and RX
}

void
uart_send_data (uint16_t Data)
{
  /* Transmit Data */
  UART0_DR_R = (Data & (uint16_t)0x00FF);
}

int
fputc (int ch, FILE *f)
{

  uart_send_data ((uint8_t)ch);
  while ((UART0_FR_R & (1 << 5)) != 0)
    ;
  return (ch);
}
