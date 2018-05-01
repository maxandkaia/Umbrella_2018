#ifndef PALETTES_H
#define PALETTES_H

#include "color_utils.h"
#include "palette_data.h"

enum PalChoice {
	kClouds = 0,
	kRainbow,
	kParty,
	kPurpleFly,
	kIris,
	kMadras,
	kLailah,
	kHope,
	kCompassion,
	kRepose,
	kMermaid,
	kSerendil,
	kFlame,
	kDaisyFae,
	kFaeCat,
	kFireIce,
	kHangOn,
	kMistress,
	kOtis,
	kScoutie,
	kSunlitWave,
	kTrove,
	kBlackhorse,
	kPlumbago,
	kSchwarzwald,
	kNrwc,
	kEpochs,
	kSky04,
	kSky12,
	kSky34,
	kNumPalettes
};


// An array of palette pointers so we can randomly choose one
//#define kNumPalettes 64
CRGBPalette16 palettes[kNumPalettes];

// when we want to blend to a new palette, we assign to nextPalette, then use nblend to modify the target palette over time.
// all color fetch functions use the final palette in some way or another
CRGBPalette256 nextPalette;
CRGBPalette256 curPalette;
CRGBPalette256 finalPalette;

// This generates in mem copies of all of the progmem palettes that we actually want
// These get upscaled into 256 entry palettes when assigned to nextPalette
void GenerateGlobalPalettes()
{
  palettes[kClouds] = CloudColors_p;
  palettes[kRainbow] = RainbowColors_p;
  palettes[kParty] = PartyColors_p;
  palettes[kPurpleFly] = purplefly_gp;
  palettes[kIris] = iris_gp;
  palettes[kMadras] = madras_gp;
  palettes[kLailah] = lailah1_gp;
  palettes[kHope] = hopegoddess_gp;
  palettes[kCompassion] = angelcompassion_gp;
  palettes[kRepose] = angelrepose_gp;
  palettes[kMermaid] = aquamarinemermaid_gp;
  palettes[kFlame] = flame_gp;
  palettes[kSerendil] = serendil_gp;
  palettes[kDaisyFae] = daisyfae_gp;
  palettes[kFaeCat] = faecat_gp;
  palettes[kFireIce] = fireandice_gp;
  palettes[kHangOn] = hangonfatboy_gp;
  palettes[kMistress] = mistressnight_gp;
  palettes[kOtis] = otis_gp;
  palettes[kScoutie] = scoutie_gp;
  palettes[kSunlitWave] = sunlitwave_gp;
  palettes[kTrove] = trove_gp;
  palettes[kBlackhorse] = blackhorse_gp;
  palettes[kPlumbago] = plumbago_gp;
  palettes[kSchwarzwald] = schwarzwald_gp;
  palettes[kNrwc] = nrwc_gp;
  palettes[kEpochs] = epochs_gp;
  palettes[kSky04] = sky_04_gp;
  palettes[kSky12] = sky_12_gp;
  palettes[kSky34] = sky_34_gp;

// Make sure all palettes are initialized
  finalPalette = palettes[kSchwarzwald];
  curPalette = finalPalette;
  nextPalette = curPalette;
}




#endif
