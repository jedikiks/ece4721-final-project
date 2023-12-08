#include "../inc/lcd.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/util.h"
#include <stdint.h>

#define LCD_ADDR 0x27

// IO direction
#define _IN() (GPIO_PORTF_DIR_R &= ~MASK (SDA_PIN)) // PF2 as input
#define _OUT() (GPIO_PORTF_DIR_R |= MASK (SDA_PIN)) // PF2 as out

// IO control

#define I2C_SCL(a)                                                            \
  (a ? (GPIO_PORTF_DATA_R |= MASK (SCL_PIN))                                  \
     : (GPIO_PORTF_DATA_R &= ~MASK (SCL_PIN))) // SCL I2C clock

#define I2C_SDA(b)                                                            \
  (b ? (GPIO_PORTF_DATA_R |= MASK (SDA_PIN))                                  \
     : (GPIO_PORTF_DATA_R &= ~MASK (SDA_PIN))) // SDA I2C data

#define IN_SDA() ((GPIO_PORTF_DATA_R & MASK (SDA_PIN)) ? 1 : 0) // read

static int i = 0, change = 1;

// delay
void delay_us (int delay);

// I2c
void I2C_Stop (void);
void I2C_Start (void);
void I2C_Wait_ACK (void);
void I2C_write_byte (uint8_t data);

void I2C_write_data (uint8_t data);

// Tri. Initilization
void LCD_Init (void);

void
I2C_Init (void)
{
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOB; // actvie clock for PB;

  GPIO_PORTB_DIR_R |= (MASK (SCL_PIN) | MASK (SDA_PIN));
  GPIO_PORTB_DEN_R |= (MASK (SCL_PIN) | MASK (SDA_PIN));

  I2C_SCL (1);
  I2C_SDA (1);

  I2C_Start ();
  I2C_write_byte (LCD_ADDR);
  I2C_Wait_ACK ();
}

void
I2C_Start (void)
{
  _OUT ();
  I2C_SCL (1);
  I2C_SDA (1);
  delay_us (5);
  I2C_SDA (0);
  delay_us (5);
  I2C_SCL (0);
}

void
I2C_Stop (void)
{
  _OUT ();
  I2C_SCL (0);
  I2C_SDA (0);
  delay_us (5);
  I2C_SCL (1);
  I2C_SDA (1);
  delay_us (5);
}

void
I2C_Wait_ACK (void)
{
  uint16_t error = 0;
  _IN ();
  I2C_SDA (1);
  delay_us (3);
  I2C_SCL (1);
  delay_us (3);
  while (IN_SDA ())
    {
      error++;
      if (error > 350)
        {
          I2C_Stop ();
        }
    }
  I2C_SCL (0);
}

void
I2C_write_byte (uint8_t data)
{
  uint8_t d;
  _OUT ();
  I2C_SCL (0);
  for (d = 0; d < 8; d++)
    {
      I2C_SDA (((data & 0x80) >> 7));
      data <<= 1;
      delay_us (3);
      I2C_SCL (1);
      delay_us (3);
      I2C_SCL (0);
      delay_us (3);
    }
}

void
I2C_Init (void)
{
}

void
I2C_write_data (uint8_t data)
{
  I2C_write_byte (LCD_ADDR); // write register: was 0x40
  I2C_Wait_ACK ();
  I2C_write_byte (((uint8_t)(i >> 4))
                  & 0xFF); //(the first 1 byte of 12 bits value)
  I2C_Wait_ACK ();
  I2C_write_byte (
      ((uint8_t)((i << 4)) & 0xF0)); //(the last 4 bit of the value)
  I2C_Wait_ACK ();
}

