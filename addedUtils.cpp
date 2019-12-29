
#include "FastLED.h"
#include "pixeltypes.h"
#include "fastled_progmem.h"
#include "math.h"
#include "stdlib.h"

//What I have learned about this specific program:
	//If there is something defined in the main program as far as LEDS, don't try to redefine

FASTLED_NAMESPACE_BEGIN

addedUtils::addedUtils(struct CRGB* leds, int numberLEDS, int cycles){
	this->leds = leds;
	this->numberLEDS = numberLEDS;
	this->cycles = cycles;
}

CRGB addedUtils::colorSelector(int colorNumber, int powerScale){
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

void addedUtils::scroll(int cycles){
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
	symmetricStrip();
	//leds(numberLEDS/2,numberLEDS-1) = leds(numberLEDS/2 - 1 ,0);
    FastLED.show();
    FastLED.delay(20);
    j++; //increment j to reach the next static color
  }
}

void addedUtils::symmetricStrip(){
	for(int i = 0; i < (numberLEDS/ 2); i++){
		leds[numberLEDS-i-1] = leds[i];
	}
}

void addedUtils::stackLights(struct CRGB color0, struct CRGB color1, struct CRGB color2){
  int notLit = numberLEDS - 1; //accounts for odd numbers
  int currentColor = 0;
  bool stringFilled = false;
  bool firstCycle = true;
  fill_solid(leds, numberLEDS, CRGB(0, 0, 0));
  while(!stringFilled){
    for(int i = 0; i <= notLit / 2; i++){
      switch(currentColor){
        case 0: leds[i] = color0;
        break;
        case 1: leds[i] = color1;
        break;
        case 2: leds[i] = color2;
        break;
      }
       //set turquoise
        if(i-3 >= 0){
          if(i-3 < (notLit/2) - 2){ //determines how many LEDs to keep up, allowing the stack effect.
          leds[i-3] = CRGB(0, 0, 0); //turn off anything that is no longer within two spaces behind i
        }
      }
      symmetricStrip();
      FastLED.show();
    FastLED.delay(10);
    }
    currentColor++;
    if(currentColor >= 3){
      currentColor = 0;
    }
    FastLED.delay(500);
    
    notLit -= 6;
    if(notLit <= 0){
      stringFilled = true;
    }
  }
  breathing(true);
} 

void addedUtils::breathing(){
  breathing(false);
}

void addedUtils::breathing(bool external){ //cycles between all primary and secondary colors
  int powerScale = 0;
  int increment = 1;
  bool upIncrement = true;
  int colorNumber = 0;
  int numCycles = 0;
  bool firstCycle = true;
  bool done = false;
  struct CRGB* newColor = new struct CRGB[numberLEDS];
  for(int i = 0; i <= numberLEDS; i++){
	  newColor[i] = leds[i];
  }
  while(1){
    if(numCycles == cycles + 1){
      break;
    }
    if(!external){
      struct CRGB singleColor = colorSelector(colorNumber, powerScale);
      for(int b = 0; b <= numberLEDS; b++){
        leds[b] = singleColor;
      }
    }
    else{
      if(firstCycle){
        powerScale = 100;
      }
      colorScale(powerScale, newColor);
    }
    FastLED.show();
      
    if(powerScale == 100){ //float comparison picks up random values and can never find an equivalence.
      FastLED.delay(100);
      upIncrement = false;
    }
    if(powerScale == 0){
      if(!firstCycle){
        colorNumber++;
      }
      if(colorNumber >= 6){
        colorNumber = 0;
      }
      FastLED.delay(200);
      upIncrement = true;
      numCycles++;
    }
    else{
      FastLED.delay(20);
    }

    if(upIncrement){
      powerScale = powerScale + increment;
    }
    else{
      powerScale = powerScale - increment;
    }
    firstCycle = false;
    //Serial.println("Value of powerScale is: ");
    //Serial.println(powerScale);
  }
  delete[] newColor;
  
}

void addedUtils::colorScale(int powerScale, struct CRGB* newColor){
	for(int i = 0; i <= numberLEDS; i++){
		leds[i].r = newColor[i].r * powerScale * .01;
		leds[i].g = newColor[i].g * powerScale * .01;
		leds[i].b = newColor[i].b * powerScale * .01;
	}
}

FASTLED_NAMESPACE_END
