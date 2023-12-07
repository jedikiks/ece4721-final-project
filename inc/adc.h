#ifndef ADC_H_
#define ADC_H_

/* Pinouts */
#define JS_SW_PIN 0 // Joystick switch <= PE0
#define VOUT_PIN 1 // Output voltage <= PE1
#define JS_X_PIN 3 // Joystick x <= PE3
#define JS_Y_PIN 2 // Joystick y <= PE2 

#define ADC_NUM (0) // ADC0

/* Freq + dty inc/dec step */
#define STEP 5

/* Joystick calibration */
#define JS_X_R 3300
#define JS_X_L 300
#define JS_Y_U 300 
#define JS_Y_D 3300

void adc_init (void);
void adc_check_bounds (void);
double adc_read_vout (void);
int js_get_input (void);
unsigned char debounce (unsigned char input);
double adc_read (void);
int
js_getvalue (unsigned char dir);

#endif // ADC_H_
