#ifndef _LCD_H_
#define _LCD_H_

#define SCL_PIN (2) // SCL <= PB2
#define SDA_PIN (3) // SDA <= PB3

void I2C_Init (void);
void lcd (void);

void lcd_clear (void);
void LCD4bits_Init (void); // Initialization of LCD Dispaly
void LCD_Write4bits (unsigned char,
                     unsigned char); // Write data as (4 bits) on LCD
void LCD_WriteString (char *);       // Write a string on LCD
void LCD4bits_Cmd (unsigned char);   // Write command
void LCD4bits_Data (unsigned char);  // Write a character
void LCD_DisplayString (char *str);
#endif
