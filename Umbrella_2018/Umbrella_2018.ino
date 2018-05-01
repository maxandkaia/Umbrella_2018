
#include "FastLED.h"
#include "power_mgt.h"

#define NUM_LEDS 64

// APA102 color order is BGR
// Using TWO outputs only, replicated to 4 legs each for a total of 8 legs on the umbrella.
CRGB leds[2][NUM_LEDS];

CLEDController* controllers[2];

// Includes of other modules
#include "led_utils.h"
#include "effect.h"

// Command processing and effect sequence control to drive the whole show
// command sequences can come from the Serial line or from a file or data
#include "controls.h"
#include "commands.h"

unsigned long lastmics = 0;
unsigned long lastmillis = 0;
int timeTillPrint = 10000;
int timeTillRender = 16;

void setup ()
{
  Serial.begin(115200);

  // Using two SPI pinsets. BUT: No need to use OE pins on buffers, as these two are not overloaded
  controllers[0] = &FastLED.addLeds<APA102, 7, 13, BGR, DATA_RATE_MHZ(12)>(leds[0], NUM_LEDS);  
  controllers[1] = &FastLED.addLeds<APA102, 11, 14, BGR, DATA_RATE_MHZ(12)>(leds[1], NUM_LEDS);  
  
  // limit my draw to 20A at 5v of power draw (limit of my big 60A supply) 
  // NOTE! Use quarter the power, as fastLED thinks we only have 2 controllers, i.e. only one fourth of the umbrella, which is 8 strips!!
  set_max_power_in_volts_and_milliamps(5, 5000);

  // Create a bunch of palettes to use
  GenerateGlobalPalettes();

  // Init all the effects and the controller that drives/animates them
  controller.Init();   



  // slow start so we can get the monitor up
  delay(1000);
  FastLED.setBrightness(globalBrightness);

  // Turn off dither so low brightness stuff doesn't flicker so bad?
  FastLED.setDither( 0 );

  SetAll(leds[0],CRGB::Red);
  SetAll(leds[1],CRGB::Green);
  ShowAll();

  delay(10000);

}

int frameCount = 0;
int renderCount = 0;

void loop () 
{
  frameCount++;

  // Do timing work
  unsigned long curmics = micros();
  unsigned long deltamics = curmics - lastmics;
  lastmics = curmics;
  unsigned long curmillis = millis();
  unsigned long deltamillis = curmillis - lastmillis;
  lastmillis = curmillis;
  timeTillPrint -= deltamillis;
  timeTillRender -= deltamillis;

  // Update the sensors and the fusion filter
  //  UpdateMotion();

  // Handle inputs
  ProcessInput();

  
  if (timeTillPrint <= 0)
  {
    timeTillPrint = 10000;

    Serial.print("frames per sec: "); Serial.println(frameCount/10);
    frameCount = 0;
    Serial.print("rendered frames: "); Serial.println(renderCount/10);
    renderCount = 0;

    Serial.printf("brightness: %d\n",globalBrightness);

  }

  // Animate the controller. This will animate any active effects, process animated palettes, lfo's, and do random or scripted switching of effects and animation of parameters
  controller.Animate(deltamics);

  // Render all active buffers and mixdown, then show with power limits applied
  if (timeTillRender <= 0)
  {
    renderCount++;
    timeTillRender = 16 + timeTillRender;
    controller.Render();
    ShowAll();
  }

}
