
#include "FastLED.h"
#include "pixeltypes.h"
#include "fastled_progmem.h"
#include "math.h"
#include "stdlib.h"

//What I have learned about this specific program:
	//If there is something defined in the main program as far as LEDS, don't try to redefine

FASTLED_NAMESPACE_BEGIN

CRGB colorSelector(int colorNumber, int powerScale){
	switch(colorNumber){
        case 0: return CRGB(0,100*(powerScale*.01),0);//red
        case 1: return CRGB(100*(powerScale*.01),100*(powerScale*.01),0);
        case 2: return CRGB(100*(powerScale*.01),0,0);//green
        case 3: return CRGB(100*(powerScale*.01),0,100*(powerScale*.01));
        case 4: return CRGB(0,0,100*(powerScale*.01));//blue
        case 5: return CRGB(0,100*(powerScale*.01),100*(powerScale*.01));
        default: return CRGB(0,0,100*(powerScale*.01));
      }
}

void colorScale(int powerScale, struct CRGB* leds, struct CRGB* newColor, int numberLEDS){
	for(int i = 0; i <= numberLEDS; i++){
		leds[i].r = newColor[i].r * powerScale * 0.01;
		leds[i].g = newColor[i].g * powerScale * 0.01;
		leds[i].b = newColor[i].b * powerScale * 0.01;
	}
}

void scroll(int cycles, struct CRGB* leds, int numberLEDS){
  struct CRGB newColor[numberLEDS];
  for(int i = 0; i <= numberLEDS; i++){
	  newColor[i] = leds[i];
  }
  int numCycles = 0;
  int j = 1;
  while(1){
	if(j == numberLEDS + 1){//if j becomes greater than the amount of LEDS, reset to 1 to take from beginning of array again
		j = 1;
		numCycles++;
		if(numCycles == cycles){
		return;
		}
	}
	for(int i = 0; i <= numberLEDS; i++){
		int diff = i - j;
		if(diff < 0){ //if the difference is below zero, need to access element that is 'diff' less than number of leds
			leds[i] = newColor[numberLEDS - abs(diff)];
		}
		else{
			leds[i] = newColor[diff]; //if the difference isn't less than 0 (EX: i = 10, j = 2), simply grab the correct color.
		}
	}
	for(int i = 0; i <= numberLEDS/ 2; i++){
		leds[numberLEDS-i] = leds[i];
	}
	//leds(numberLEDS/2,numberLEDS-1) = leds(numberLEDS/2 - 1 ,0);
    FastLED.show();
    FastLED.delay(20);
    j++; //increment j to reach the next static color
  }
}

FASTLED_NAMESPACE_END
