#include "../inc/util.h"

int
map (int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void
ms_delay (unsigned int t)
{
  unsigned int i, j;
  for (i = 0; i < t; i++)
    for (j = 0; j < 4000; j++)
      ;
}
