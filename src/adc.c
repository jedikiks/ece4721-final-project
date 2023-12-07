#include "../inc/adc.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/util.h"
#include <stdint.h>

double adc_read (void);

void
adc_init (void)
{
  /* Enable Clock to ADC0 and GPIO pins*/
  SYSCTL_RCGCGPIO_R
      |= (SYSCTL_RCGC2_GPIOE
         | SYSCTL_RCGC2_GPIOB);        /* Enable Clock to GPIOE or PE3/AN0 */
  SYSCTL_RCGCADC_R |= MASK (ADC_NUM); // activate ADC0 clock

  /* initialize PE3 for AIN0 input*/
  GPIO_PORTE_DIR_R &= ~(MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                        | MASK (VOUT_PIN)); /* set inputs portE*/
  GPIO_PORTB_DIR_R &= ~(MASK (JS_SW_PIN));  /* set inputs portB*/
  GPIO_PORTE_AFSEL_R |= (MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                         | MASK (VOUT_PIN)); /* enable alternate function */
  GPIO_PORTE_DEN_R &= ~(MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                        | MASK (VOUT_PIN)); /* disable digital function */
  GPIO_PORTB_DEN_R |= MASK (JS_SW_PIN);     /* enable digital function */
  GPIO_PORTE_AMSEL_R |= (MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                         | MASK (VOUT_PIN)); /* enable analog function */

  /* initialize sample sequencer3 */
  ADC0_ACTSS_R &= ~MASK (3);            /* disable SS3 during configuration */
  ADC0_EMUX_R &= ~((uint32_t)(0xF000)); /* software trigger conversion */
  ADC0_SSMUX3_R = 0;                    /* get input from channel 0 */
  ADC0_SSCTL3_R
      |= (MASK (1)
          | MASK (2)); /* take one sample at a time, set flag at 1st sample */
  ADC0_ACTSS_R |= MASK (3); /* enable ADC0 sequencer 3 */
}

double
adc_read (void)
{
  double pot_val;

  ADC0_PSSI_R
      |= MASK (3); /* Enable SS3 conversion or start sampling data from AN0 */
  while ((ADC0_RIS_R & 0x08) == 0) /* Wait until sample conversion completed*/
    ;

  pot_val
      = ADC0_SSFIFO3_R & 0xFFF; /* read adc conversion result from SS3 FIFO*/
  ADC0_ISC_R |= MASK (3);       /* clear conversion clear flag bit*/

  return pot_val;
}

void
adc_check_bounds (void)
{
  /* Freq bounds */
  if (PWM0_0_LOAD_R > 16000)
    PWM0_0_LOAD_R = 16000;

  if (PWM0_0_LOAD_R < 16)
    PWM0_0_LOAD_R = 16;

  /* Dty bounds */
  /* TODO: check dty bounds */
}

double
adc_read_vout (void)
{
  /* Get input from channel 2 */
  ADC0_SSMUX3_R = 2;
  return (0.0008 * adc_read ());
}

int
js_get_input (void)
{
  double js_x_value, js_y_value;

  /*
  if (debounce (JS_SW_PIN) == 1)
    return 4;
    */

  /* Get input from channel 0, x */
  ADC0_SSMUX3_R = 0;
  js_x_value = adc_read ();

  if (js_x_value > JS_X_R)
    return 0;
  else if (js_x_value < JS_X_L)
    return 1;

  /* Get input from channel 1, y */
  ADC0_SSMUX3_R = 1;
  js_y_value = adc_read ();

  if (js_y_value < JS_Y_U)
    return 3;
  else if (js_y_value > JS_Y_D)
    return 2;

  return -1;
}

/* Simple debouncer for passed input. Returns 1
 * for debounced input, returns 0 for no input */
unsigned char
debounce (unsigned char input)
{
  /* These hold the current value of the switch */
  uint8_t r_initial = GPIO_PORTB_DATA_R & (MASK (input));
  uint8_t r_final = r_initial;

  /* If there's no button press then skip */
  while (r_initial == 0)
    {
      if (r_final != r_initial)
        return 1;

      r_final = GPIO_PORTB_DATA_R & (MASK (input));
      ms_delay (500);
    }

  return 0;
}
