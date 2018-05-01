#ifndef CRACKLE_H
#define CRACKLE_H
#include "commonheaders.h"

class crackle : public effect
{

    unsigned long micsperspark = ONEMIL; // one per second by default
    long micstilspark = 0;
    unsigned char palIndex = 0;

    // mode: 0 = all over, 1 = out from middle, 2 = around the dial
    uint8_t mode = 0;
    float legpos = 0.0; // goes from 0.0 to 6.0. Use the fraction to control distribution percentange to adjacent legs
    float radpos = 0.0; // goes from 0.0 to 1.0. Middle of umb to outer edge

    uint8_t legBPM = 60;
    uint8_t radBPM = 60;
    uint8_t radSpread = 12;

    uint8_t sparksToRender = 0;

  public:

    void Init()
    {
      // Nothing to do here, use the param setter routines below!!
    }

    String Identify()
    {
      return "crackle";
    }

    void Reset()
    {
      micsperspark = ONEMIL; // one per second by default
      micstilspark = 0;
      palIndex = 0;
    }

    void SetFrequency(float f)
    {
      effect::SetFrequency(f);
      // How many mics to wait til next spark
      micsperspark = (unsigned long)((1.0f / f) * 1000000.0f);
    }

    void Animate(unsigned long mics)
    {
      micstilspark -= mics;

      // Gen a spark when it is time
      if (micstilspark <= 0)
      {
        RenderSpark();
        micstilspark = micsperspark;
      }

      // Animate the frequency, spread and mode
      mode = (int)(3.0f * (float)beat16(2)/(65535.0f));

      radBPM = (int)(20.0f  * (float)beatsin16(2)/(65535.0f)) + 30; // Should be max of 50 revolutions per minute, or less than one per second. Not sure why it does not appear that way?
      legBPM = (int)(20.0f * (float)beatsin16(4)/(65535.0f)) + 10;
     
    }

    void RenderSpark()
    {
      int whichLeg = 0;
      int whichLed = 0;

      // Using the rotating value, pick either a set of legs or
      // a radius value
      radpos = (float)beat16(radBPM)/(65535.0f);
      if (radpos >= 1.0f) radpos = 0.0f;
      legpos = (float)beat16(legBPM)/(65535.0f);
      if (legpos >= 1.0f) legpos = 0.0f;

      if (mode == 0)
      {
         whichLeg = random(0,6);
         whichLed = random(0,63);
      }
      else if (mode == 1)
      {
        whichLeg = random(0,6);
        int r = (int)(radpos*64.0f);
        whichLed = (r-(radSpread/2)) + random8(radSpread);
      }
      else if (mode == 2)
      {
        whichLeg = (int)(legpos*6.0f);
        whichLed = random(0,63);
      }

      if (whichLed >= 0 && whichLed < NUM_LEDS && whichLeg >=0 && whichLeg < 6)
      {
        leds[whichLeg][whichLed] =  finalPalette[palIndex++];
      }      
    }

    void Render()
    {
        // Because we are setting the led array in the Animate function, we cannot clear the array here. Only ever fade it!
        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[0][i].nscale8(fadeunits);
          leds[1][i].nscale8(fadeunits);
          leds[2][i].nscale8(fadeunits);
          leds[3][i].nscale8(fadeunits);
          leds[4][i].nscale8(fadeunits);
          leds[5][i].nscale8(fadeunits);
        }
  }
} crackle;
#endif
