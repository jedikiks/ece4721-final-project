#include "../inc/util.h"
#include "../inc/pwm.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

void pwm_init (void);

void
pwm_init (void)
{
  /* Clock setting for PWM and GPIO port */
  SYSCTL_RCGCPWM_R |= (1UL << 0);  /* Enable clock to PWM0 module */
  SYSCTL_RCGCGPIO_R |= (1UL << 1); /* Enable system clock for PORTB */
  SYSCTL_RCC_R &= (~(1UL << 20));  /* Directly feed clock to PWM0 module
                                    * w/o pre-divider */

  /* Setting of PF2 pin for M1PWM6 channel output pin */
  GPIO_PORTB_AFSEL_R |= (1UL << 6); /* Set PB6 alternate function */
  GPIO_PORTB_PCTL_R = 0x04000000;   /* Make PB6 output */
  GPIO_PORTB_DEN_R |= (1UL << 6);   /* Set PB6 as digital */

  /* PWM0 channel config */
  PWM0_0_CTL_R &= (~(1UL << 0)); /* Disable generator 3 counter */
  PWM0_0_CTL_R &= (~(1UL << 1)); /* Select down count mode for counter 3 */
  PWM0_0_LOAD_R = 640;           /* Set load value for 25kHz */
  PWM0_0_CMPA_R = 320; /* Set duty cycle to 50% using half load value */
  PWM0_0_GENA_R |= (1UL << 2) | (1UL << 3)
                   | (1UL << 7); /* Set pwm output when counter
                                  * reloads and clear when match */
  PWM0_0_CTL_R |= (1UL << 0);    /* Enable generator 3 counter */
  PWM0_ENABLE_R |= (1UL << 0);   /* Enable PWM1 channel 6 output */
}
