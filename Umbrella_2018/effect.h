#ifndef EFFECT_H
#define EFFECT_H
#include "color_utils.h"
class effect
{
  public:

  	enum ColorMode
  	{
  		kSingleColor=0,
  		kFromPalette=1,
  		kFromPaletteByHeight=2,
  		kFromPaletteByAngle=3,
  		kFromPaletteRandom
  	};

  	enum MotionMode
  	{
  		kOneWayDown=0,
  		KDownAndBack=1,
  		kFromBothEnds=2,
  		kOneWayBack=3
  	};

  	enum ClearMode
  	{
  		kClear=0,
  		kFade=1
  	};

  	enum MirrorMode
  	{
  		kNone=0,
  		kMirror,
  		kFlipAndMirror
  	};

	// Some basic controls for most effects
  	float frequency;
  	float speed;
  	float width;
  	float fadetime;
  	uint8_t fadeunits; // for use with nscale8 to fade the effect over time
  	uint8_t pal;
  	CHSV color;
  	ColorMode colormode;
  	MotionMode motionmode;
  	ClearMode clearmode;
  	MirrorMode mirrormode;

  	bool running = false;

  	effect()
  	{
  		SetFrequency(1.0f);
  		SetSpeed(1.0f);
  		SetWidth(1.0f);
  		SetFadeTime(1.0f);
 		SetColor(CHSV(0,0,255));
 		SetColorMode(kSingleColor);
 		SetClearMode(kClear);
 		SetMirrorMode(kNone);
  	}

    virtual void Animate(unsigned long micros);
    virtual void Render();
    virtual void SetClearMode(ClearMode mode) {clearmode = mode;} // Clear to black before rendering each frame, or fade old buffer contents out over time (leave trails)
    virtual void SetMirrorMode(MirrorMode mode) {mirrormode = mode;}

	// These all have default impls so that we can safely call them on effects that don't use them
    // NOTE: Make sure these routines can be called safely while an effect is active!! 
    virtual void SetFrequency(float f) {frequency=f;}; // Usually in leds/sec or launches/sec etc
    virtual void SetSpeed(float s) {speed=s;}; // Usually in leds/sec or normalized param/sec
    virtual void SetWidth(float w) {width=w;}; // Usually in leds, or norm param
    
    virtual void SetFadeTime(float t)
    {
		fadetime=t;
		// Every second we will fade this amount
		float fadepersec = 1.0f/t;
		float fadeperframe = fadepersec / 60.0f;
		fadeunits = (int)(255.0f*(1.0f-fadeperframe));

    }; // The time a newly lit led should take to fade to black

    virtual void SetPalette(uint8_t p) {pal=p;}; // Set the palette to use
    virtual void SetColor(const CHSV c) {color=c;}; // Color to use (if using single color)
    virtual void SetColorMode(ColorMode m) {colormode=m;}; // Change how the effect picks colors (single, from palette, random, ???)

    // Some funcs used by only a few effects
    virtual void SetSpan(int N) {}; // Set some kind of span or interval (N for mod N effects for example)

    virtual String Identify() = 0; // Every effect must name itself
    virtual void Start() {running = true;};
    virtual void Stop() {running = false;};
    virtual void Reset() = 0; // Every effect must support Reset so we can start setting params and restart

    void PrintState()
    {
    	Serial.print(Identify()); Serial.println(": ");
    	Serial.print("    "); Serial.print("frequency: "); Serial.println(frequency);
    	Serial.print("    "); Serial.print("speed: "); Serial.println(speed);
    	Serial.print("    "); Serial.print("width: "); Serial.println(width);
    	Serial.print("    "); Serial.print("fadetime: "); Serial.println(fadetime);
    	Serial.print("    "); Serial.print("fadeunits: "); Serial.println(fadeunits);
    	Serial.print("    "); Serial.print("pal: "); Serial.println(pal);
    	Serial.print("    "); Serial.print("colormode: "); Serial.println(colormode);
    	Serial.print("    "); Serial.print("motionmode: "); Serial.println(motionmode);
    	Serial.print("    "); Serial.print("clearmode: "); Serial.println(clearmode);
    	Serial.printf("    mirrormode: %i\n",mirrormode);
    }

};
#endif
