#include "../inc/util.h"
#include "../inc/pwm.h"
#include "../inc/adc.h"
#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>

int
main (void)
{
  adc_init ();
  pwm_init ();

  double js_x_value, js_y_value, vout;
//
  while (1)
    {
      /* Get input from channel 0 */
      ADC0_SSMUX3_R = 0;
      js_x_value = adc_read ();

      if (js_x_value > 3660)
        PWM0_0_CMPA_R += STEP;
      else if (js_x_value < 40)
        PWM0_0_CMPA_R -= STEP;

      /* FIXME: check dty bounds */

      /* Get input from channel 1 */
      ADC0_SSMUX3_R = 1;
      js_y_value = adc_read ();

      if (js_y_value < 160)
        {
          PWM0_0_LOAD_R += STEP * 2;
          PWM0_0_CMPA_R = PWM0_0_LOAD_R / 2.0f;
        }
      else if (js_y_value > 3520)
        {
          PWM0_0_LOAD_R -= STEP * 2;
          PWM0_0_CMPA_R = PWM0_0_LOAD_R / 2.0f;
        }

      /* Check bounds */
      if (PWM0_0_LOAD_R > 16000)
        PWM0_0_LOAD_R = 16000;

      if (PWM0_0_LOAD_R < 16)
        PWM0_0_LOAD_R = 16;

      /* Get input from channel 2 */
      ADC0_SSMUX3_R = 2;
      vout = 0.0008 * adc_read ();

      ms_delay (10);
    }
}
