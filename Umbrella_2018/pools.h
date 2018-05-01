#ifndef POOLS_H
#define POOLS_H
#include "commonheaders.h"


class pools : public effect
{
  public:
    float nodes[2][5]; // units are frontpalette space (0-255)
    float vels[2][5];  // in frontpalette units per second
    int locs[2][5]; // the node locations in strip space
    int m_length;


    static const unsigned long micsPerModechange = 1000000 * 20; // 30 seconds between effect changes (change to random later)
    long micsTilModeChange = micsPerModechange;
    uint8_t curMode = 0; // 0 = slow, 1 = fast, 2 = rythmic

    void Init()
    {
      m_length = NUM_LEDS;
      // For now, random vels
      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 5; i++)
        {
          int r = random(10, 255);
          vels[leg][i] = (float)(r);
          locs[leg][i] = i * (m_length / 8);
          nodes[leg][i] = 0;
        }
        locs[leg][4] = m_length - 1; // put last node at the end
      }

      SlowVels();
    }


    String Identify()
    {
      return "pools";
    }

    void Reset()
    {
      // Nothing for now, but soon...
    }


    void SlowVels()
    {
      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 5; i++)
        {
          int r = random(10, 60);
          vels[leg][i] = (float)(r);
        }
      }
    }

    void FastVels()
    {
      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 5; i++)
        {
          int r = random(20, 100);
          vels[leg][i] = (float)(r);
        }
      }
    }

    void Rythmic()
    {
      // For now, random vels
      int v[5] = {10, 86, 47, 86, 10};
      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 5; i++)
        {
          int n;
          if (i % 2)
          {
            n = 0;
          }
          else
          {
            n = 255;
          }
          vels[leg][i] = (float)(v[i]);
          nodes[leg][i] = (float)(n);
        }
      }

    }

    void Animate(unsigned long mics)
    {
      float dt = (float)(mics) / (float)ONEMIL;

      // periodically change the mode
      micsTilModeChange -= mics;
      if (micsTilModeChange <= 0)
      {
        curMode++;
        if (curMode >= 3) curMode = 0;
        if (curMode == 0) SlowVels();
        if (curMode == 1) FastVels();
        if (curMode == 2) Rythmic();
        micsTilModeChange = micsPerModechange;
      }

      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 5; i++)
        {
          nodes[leg][i] += vels[leg][i] * dt;
          if (nodes[leg][i] < 0)
          {
            nodes[leg][i] = -nodes[leg][i]; // bounce
            vels[leg][i] = -vels[leg][i];
          }
          if (nodes[leg][i] > (float)(255))
          {
            nodes[leg][i] = (float)(511) - nodes[leg][i]; // bounce
            vels[leg][i] = -vels[leg][i];
          }
        }
      }
    }

    void Render()
    {


      for (int leg = 0; leg < 2; leg++)
      {
        for (int i = 0; i < 4; i++)
        {
          // interpolate from node i to i+1 in frontpalette space
          float startp = nodes[leg][i];
          float endp = nodes[leg][i + 1];
          float pdelta = (endp - startp) / (float)(locs[leg][i + 1] - locs[leg][i]);
          float curp = startp;
          for (int l = locs[leg][i]; l <= locs[leg][i + 1]; l++)
          {
            int palindex = (int)(curp);
            if (palindex < 0) palindex = 0;
            if (palindex > 255) palindex = 255;
            leds[leg][l] = finalPalette[palindex];
            curp += pdelta;
          }
        }
      }


    }

} pools;
#endif
