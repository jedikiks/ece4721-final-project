#include "../inc/util.h"
#include "../inc/adc.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

void
adc_init (void)
{
  /* Enable Clock to ADC0 and GPIO pins*/
  SYSCTL_RCGCGPIO_R
      |= SYSCTL_RCGC2_GPIOE;          /* Enable Clock to GPIOE or PE3/AN0 */
  SYSCTL_RCGCADC_R |= MASK (ADC_NUM); // activate ADC0 clock

  /* initialize PE3 for AIN0 input*/
  GPIO_PORTE_DIR_R &= ~(MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                        | MASK (VOUT_PIN)); /* set inputs */
  GPIO_PORTE_AFSEL_R |= (MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                         | MASK (VOUT_PIN)); /* enable alternate function */
  GPIO_PORTE_DEN_R &= ~(MASK (JS_X_PIN) | MASK (JS_Y_PIN)
                        | MASK (VOUT_PIN)); /* disable digital function */
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
