#ifndef COMMANDS_H
#define COMMANDS_H


static const int MAX_BUF = 64;
static const int MAX_ARGS = 12;
int numInput = 0;
char inputBuffer[MAX_BUF];
char *command;
char *args[MAX_ARGS];
int numArgs = 0;

static const int BRIGHT_PIN = 23;

#include "commonheaders.h"
#include "controls.h"

effect* selectedEffect = NULL;
effect* EffectByName(const char* name)
{
  Serial.print("Looking up effect: "); Serial.println(name);

  if (strcmp(name,"crackle") == 0) return &crackle;
  else if (strcmp(name,"pools") == 0) return &pools;
 else if (strcmp(name,"pmarch") == 0) return &pmarch;
  else if (strcmp(name,"random") == 0)
  {
    int selection = random(0,numEffects);
    return effectTable[selection];
  }
  else
  {
    Serial.println("Unrecognized effect!");
   return NULL;
  }

}

void SelectEffect(const char* name)
{
  selectedEffect = EffectByName(name);
}

// Set an effect to a slot, and also select it for further manipulation
void SetEffect(int slot, const char *name)
{
  if (slot < 0 || slot > 2) return;
  currenteffects[slot] = EffectByName(name);
  selectedEffect = currenteffects[slot];
}

void SetClearMode(int slot, int mode)
{
  if (slot < 0 || slot > 2) return;
  if (mode < 0 || mode > effect::kFade) return;
  if (currenteffects[slot] != NULL)
  {
    Serial.print("Setting clearmode: "); Serial.println(mode);
    currenteffects[slot]->SetClearMode(static_cast<effect::ClearMode>(mode));
  }
}

void SetFadeTime(int slot, float fadetime)
{
  if (slot < 0 || slot > 2) return;
  if (currenteffects[slot] != NULL)
  {
    Serial.print("Setting fadetime: "); Serial.println(fadetime);
    currenteffects[slot]->SetFadeTime(fadetime);
  }
}

void SetSpeed(int slot, float speed)
{
  if (slot < 0 || slot > 2) return;
  if (currenteffects[slot] != NULL)
  {
    Serial.print("Setting speed: "); Serial.println(speed);
    currenteffects[slot]->SetSpeed(speed);
  }
}

void SetFrequency(int slot, float freq)
{
  if (slot < 0 || slot > 2) return;
  if (currenteffects[slot] != NULL)
  {
    Serial.print("Setting freq: "); Serial.println(freq);
    currenteffects[slot]->SetFrequency(freq);
  }
}

void SetWidth(int slot, float w)
{
  if (slot < 0 || slot > 2) return;
  if (currenteffects[slot] != NULL)
  {
    Serial.print("Setting width: "); Serial.println(w);
    currenteffects[slot]->SetWidth(w);
  }
}

void SplitCommand()
{

  numArgs = 0;
  command = strtok (inputBuffer,"  ,");
//  Serial.print("command is: "); Serial.println(command);
  if (command == NULL) return;

  do {
    args[numArgs] = strtok (NULL, " ,");
    numArgs++;
  } while (args[numArgs-1] != NULL); 
  numArgs--;
  Serial.print("numargs is: "); Serial.println(numArgs);
}

void PrintState()
{
  Serial.print("slot0: ");
  if (currenteffects[0] != NULL)
  {
    currenteffects[0]->PrintState();
  }
  else Serial.println("NULL");

  Serial.print("slot1: ");
  if (currenteffects[1] != NULL)
  {
    currenteffects[1]->PrintState();
  }
  else Serial.println("NULL");

  Serial.print("selectedEffect: ");
  if (selectedEffect != NULL)
  {
    Serial.println(selectedEffect->Identify());
  }
  else Serial.println("NULL");

}

void DoCommand()
{
  // Split the string into command and args
  SplitCommand();

  Serial.print("Command: "); Serial.println(command);
  for (int i=0; i<numArgs; i++)
  {
    Serial.print("Arg: "); Serial.println(args[i]);
  }

  if (strcmp(command,"speed") == 0)
  {
    float S = atof(args[0]);
    Serial.println("doing speed command");
    Serial.println(S);
    if (selectedEffect)
      selectedEffect->SetSpeed(S);
  }
  else if (strcmp(command,"span") == 0)
  {
    int S = atoi(args[0]);
    Serial.println("doing span command");
    Serial.println(S);
    if (selectedEffect)
      selectedEffect->SetSpan(S);
  }
  else if (strcmp(command,"width") == 0)
  {
    float W = atof(args[0]);
    Serial.println("doing width command");
    Serial.println(W);
    if (selectedEffect)
      selectedEffect->SetWidth(W);
  }
  else if (strcmp(command,"pal") == 0)
  {
    float p = atoi(args[0]);
    Serial.println("doing pal command");
    Serial.println(p);
    if (selectedEffect)
      selectedEffect->SetPalette(p);
  }
  else if (strcmp(command,"freq") == 0)
  {
    float F = atof(args[0]);
    Serial.println("doing freq command");
    Serial.println(F);
    if (selectedEffect)
      selectedEffect->SetFrequency(F);
  }  
  else if (strcmp(command,"fadetime") == 0)
  {
    float F = atof(args[0]);
    Serial.println("doing fadetime command");
    Serial.println(F);
    if (selectedEffect)
      selectedEffect->SetFadeTime(F);
  }
  else if (strcmp(command,"bright") == 0)
  {
    int b = atoi(args[0]);
    Serial.println("doing bright command");
    Serial.println(b);
    if (b > 255) b = 255;
    if (b < 0) b = 0;
    globalBrightness = b;

    FastLED.setBrightness(globalBrightness);
  }
  else if (strcmp(command,"color") == 0)
  {
    // We need 3 args for a color
    if (numArgs != 3) goto ERR;
    float H = atoi(args[0]);
    float S = atoi(args[1]);
    float V = atoi(args[2]);
    Serial.println("doing color command");
    if (selectedEffect)
      selectedEffect->SetColor(CHSV(H,S,V));
  }
  else if (strcmp(command,"select") == 0)
  {
    Serial.println("doing select command");
    SelectEffect(args[0]);
  }
  else if (strcmp(command,"selectslot") == 0)
  {
    Serial.println("doing selectslot command");
    int e = atoi(args[0]);
    if (e >=0 && e < 2)
      selectedEffect = currenteffects[e];
  }
  else if (strcmp(command,"seteffect") == 0)
  {
    int slot = atoi(args[0]);
    if (slot < 0 || slot > 1) goto ERR;
    Serial.println("doing seteffect command");
    SetEffect(slot,args[1]);
  }
  else if (strcmp(command,"clearmode") == 0)
  {
    Serial.println("doing clearmode command");
    int slot = atoi(args[0]);
    int mode = atoi(args[1]);
    SetClearMode(slot,mode);
  }
  else if (strcmp(command,"state") == 0)
  {
    Serial.println("doing state command");
    PrintState();
  }

  goto DONE;

  ERR:
  Serial.println("Syntax error in command processor!");

  DONE:

// Reset the command collectors
  numInput = 0;
  numArgs = 0;
  for (int i=0; i<MAX_ARGS; i++) args[i] = NULL;
  command = NULL;
}

int smoothVal = 0;
void CheckBrightKnob()
{
  // Use an IIR filter to smooth out the brightness value
  int val = analogRead(BRIGHT_PIN);
//  Serial.printf("analog: %d\n",val);
  smoothVal = ((smoothVal * 9) + val) / 10;
//  Serial.printf("smoothed: %d\n",smoothVal);
  globalBrightness = smoothVal/4;
  if (globalBrightness < 12) globalBrightness = 12;
  FastLED.setBrightness(globalBrightness);
}

bool ProcessInput()
{
  // Check analog input and set brightness
  CheckBrightKnob();

        while (Serial.available() > 0) {
                // read the incoming byte:
          char c = Serial.read();
          if (c == '\n')
          {
            inputBuffer[numInput++] = 0;
            Serial.println("command entered!");
            DoCommand();
            return true;
          }
          else if (numInput < MAX_BUF)
          {
                inputBuffer[numInput++] = c;
          }
          else
          {
            Serial.println("Input buffer overflow!");
            numInput = 0;
          }
        }
        return false;
}

  #endif
