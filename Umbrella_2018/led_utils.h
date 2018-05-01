
#ifndef LEDUTILS_H
#define LEDUTILS_H
#include "effect.h"

uint8_t globalBrightness = 128;


void Show()
{
  show_at_max_brightness_for_power();
}

void ShowAll()
{
  Show();
  delay(1);
}

void Blackout(CRGB* dst)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    dst[i] = CRGB(0, 0, 0);
  }
}

void SetAll(CRGB* dst, CRGB c)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    dst[i] = c;
  }
}

void SafeSetLed(CRGB* dst, int i, CRGB c)
{
	if (i>=0 && i<NUM_LEDS)
	  dst[i] = c;
}


#endif
