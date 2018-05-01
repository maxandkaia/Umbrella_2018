#ifndef CONTROLS_H
#define CONTROLS_H

#include "commonheaders.h"
#include "palettes.h"
#include "palmixer.h"

// Effects are rendered into the working buffers 0-2
effect* currenteffects[2] = {NULL, NULL};


// The effect classes
#include "Crackle.h"
#include "pools.h"
#include "pmarch.h"

static const int numEffects = 3;

effect* effectTable[numEffects] =
{
  &crackle, &pools, &pmarch
};

class controller : public effect
{

  public:

    // Palette animation timer
    static const unsigned long micsPerPalchange = 1000000 * 10; // 10 seconds between pal changes (change to random later)
    long micsTilPalChange = micsPerPalchange;
    uint8_t curPal = 0;

    // Effect change timer
    static const unsigned long micsPerEffectchange = 1000000 * 60; // 60 seconds between effect changes (change to random later)
    long micsTilEffectChange = micsPerEffectchange;
    uint8_t curEffect = 1;

    void Init()
    {
      // Make sure all palettes are initialized
      curPal = kSchwarzwald;
      finalPalette = palettes[curPal];
      curPalette = finalPalette;
      nextPalette = curPalette;

      crackle.Init();
      crackle.SetColor(hsvOrange);
      crackle.SetFrequency(200.0f);
      crackle.SetFadeTime(1.0f);
      crackle.SetClearMode(kFade);


      pools.Init();
      pools.SetClearMode(kClear);

      pmarch.Init(30.0f);
      pmarch.SetFrequency(8.0);
      pmarch.SetClearMode(kClear);
      pmarch.SetSpeed(40);
      

      currenteffects[0] = effectTable[curEffect];

    }

    String Identify()
    {
      return "controller";
    }

    void Reset()
    {
      // <<TODO>> whatever we need to do to reset basically everything!
    }

    void Animate(unsigned long mics)
    {
      float dt = (float)(mics / 1000000.0f);

      // periodically change the palette
      micsTilPalChange -= mics;
      if (micsTilPalChange <= 0)
      {
        curPal++;
        if (curPal >= kNumPalettes) curPal = 0;
        palmixer.SetNewPalette(curPal, 1.0f); // one second fade to next palette
        micsTilPalChange = micsPerPalchange;
      }

      // periodically change the effect
      micsTilEffectChange -= mics;
      if (micsTilEffectChange <= 0)
      {
        curEffect++;
        if (curEffect >= numEffects) curEffect = 0;
        currenteffects[0] = effectTable[curEffect];
        micsTilEffectChange = micsPerEffectchange;
      }

      // Always animate the palette mixer. It is gated if not active
      palmixer.Animate(dt);

      // Animate all of the current effects
      if (currenteffects[0])
        currenteffects[0]->Animate(mics);
    }

    void Render()
    {
      if (currenteffects[0])
        currenteffects[0]->Render();
    }


} controller;

#endif
