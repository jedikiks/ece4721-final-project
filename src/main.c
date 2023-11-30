#include "../inc/adc.h"
#include "../inc/pwm.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/util.h"
#include <stdint.h>

#define DEBUG

int
main (void)
{
  adc_init ();
  pwm_init ();
  //

  int t1 = 0;

  while (1)
    {
      /* Get input from channel 1 */
      switch (js_get_input ())
        {
        case 0: //l
          t1 = 0;
          PWM0_0_CMPA_R -= STEP;
          break;

        case 1: //r
          t1 = 1;
          PWM0_0_CMPA_R += STEP;
          break;

        case 2: //u
          t1 = 2;
          PWM0_0_LOAD_R += STEP * 2;
          PWM0_0_CMPA_R = PWM0_0_LOAD_R / 2.0f;
          break;

        case 3: //d
          t1 = 3;
          PWM0_0_LOAD_R -= STEP * 2;
          PWM0_0_CMPA_R = PWM0_0_LOAD_R / 2.0f;
          break;

        case 4: //b
          t1 = 4;
          PWM0_0_LOAD_R = 160;
          PWM0_0_CMPA_R = 80;
          break;

        default:
          t1 = -1;
          break;
        }

      double vout = adc_read_vout ();

      ms_delay (10);
    }
}
