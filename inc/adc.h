#ifndef ADC_H_
#define ADC_H_

/* Pinouts */
#define VOUT_PIN 1 // Output voltage <= PE1
#define JS_X_PIN 3 // Joystick_x <= PE3
#define JS_Y_PIN 2 // Joystick_y <= PE2

#define ADC_NUM (0) // ADC0

/* Freq + dty inc/dec step */
#define STEP 5


void adc_init (void);
double adc_read (void);

#endif // ADC_H_
