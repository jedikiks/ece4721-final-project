#ifndef _LCD_H_
#define _LCD_H_
int main2(void);
void LCD4bits_Init(void);													 //Initialization of LCD Dispaly
void LCD_Write4bits(unsigned char, unsigned char); //Write data as (4 bits) on LCD
void LCD_WriteString(char*);											 //Write a string on LCD 
void LCD4bits_Cmd(unsigned char);									 //Write command 
void LCD4bits_Data(unsigned char);								 //Write a character
void LCD_DisplayString(char* str);
void lcd_clear(void);
#endif