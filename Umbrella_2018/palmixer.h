#ifndef MIXER_H
#define MIXER_H
#include "palettes.h"

// In this class we animate some faders or increments
// and then support mixing of the two renderbuffers into the frontbuffer
// Which then gets replicated (or not) and displayed
class palmixer
{
  public:

  float fader = 0.0f; // Goes from 0.0f to 1.0f
  float deltaFade; // amount to fade per second
  bool active = false; // are we currently animating anything?
  fract8 fraction;
  
  
  void Animate(float dt)
  {
	if (active)
	{
		// For now just one animate mode, crossfade
		float fadeIncrement = deltaFade * dt;
		fader += fadeIncrement;
		fraction = (uint8_t)(fader * 255.0f);
		if (fader > 1.0f)
		{
			fader = 1.0f;
			fraction = 255;
			active = false;
		}

//  		Serial.printf("Blending: %d",fraction);
  		// Use blend to move toward target palette
  		for (int i=0; i<256; i++)
  		{
  			finalPalette[i] = blend(curPalette[i], nextPalette[i], fraction);
  		}
	}
  }
  
  void SetNewPalette(uint8_t newPal, float seconds)
  {
  	Serial.printf("Set new palette: %d\n", newPal);
  	// guard
  	if (newPal >= kNumPalettes) return;

  	curPalette = finalPalette;
  	nextPalette = palettes[newPal];

	fader = 0.0f;
	fraction = 0;
	deltaFade = 1.0f/seconds;
  	active = true;
  }
  
} palmixer;

#endif
