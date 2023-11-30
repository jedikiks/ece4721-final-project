#ifndef UTIL_H_
#define UTIL_H_

#define MASK(x) (1UL << x)

void ms_delay (unsigned int t);
int map (int x, int in_min, int in_max, int out_min, int out_max);

#endif // UTIL_H_
