#ifndef __INC_ADDEDUTILS_H
#define __INC_ADDEDUTILS_H

#include "FastLED.h"
#include "pixeltypes.h"
#include "fastled_progmem.h"

FASTLED_NAMESPACE_BEGIN

class addedUtils{
    private:
        struct CRGB* leds;
        int numberLEDS;
        int cycles;
    public:
        addedUtils(struct CRGB* leds, int numberLEDS, int cycles);

        CRGB colorSelector(int colorNumber, int powerScale); //choose between a number of colors to assign

        //Given two Arrays of led information, assign newColor to be the reference (unchanged)
        //and assign new values to leds based on powerScale value assigned
        void colorScale(int powerScale, struct CRGB* newColor);

        void scrollingRainbow();

        //Takes one array of led values and assigns it to the newColor array. Keeps newColor static
        //and iterates through values of itself assigning to leds one step at a time.
        void scroll(int cycles);

        //takes current CRGBArray and makes it symmetric from middle outwards (replaces arduino IDE function
        //that is really simple, but can't find a C++ function for it, so made my own with a simple for loop)
        void symmetricStrip();


        void stackLights(struct CRGB color0, struct CRGB color1, struct CRGB color2);

        void breathing(bool external);

        void breathing();

		void breathSelect(CRGB color0, CRGB color1, CRGB color2);

		void gradient_transition(struct CRGB firstColor, struct CRGB secondColor);

        CRGB colorSelectorChoose(int colorNumber, int powerScale, CRGB color0, CRGB color1, CRGB color2);


};



FASTLED_NAMESPACE_END

#endif
