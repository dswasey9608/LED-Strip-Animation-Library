#ifndef __INC_ADDEDUTILS_H
#define __INC_ADDEDUTILS_H

#include "FastLED.h"
#include "pixeltypes.h"
#include "fastled_progmem.h"

FASTLED_NAMESPACE_BEGIN

CRGB colorSelector(int colorNumber, int powerScale); //choose between a number of colors to assign

//Given two Arrays of led information, assign newColor to be the reference (unchanged)
//and assign new values to leds based on powerScale value assigned
void colorScale(int powerScale, struct CRGB* leds, struct CRGB* newColor, int numberLEDS);

void scrollingRainbow();

//Takes one array of led values and assigns it to the newColor array. Keeps newColor static
//and iterates through values of itself assigning to leds one step at a time.
void scroll(int cycles, struct CRGB* leds, int numberLEDS);


FASTLED_NAMESPACE_END

#endif
