#include <FastLED.h>
//NOTE: CRGB assignment functions as CGRB for some reason.

//Scrolling Demo Working... Uses literal D3 pin on board, WS2812 working for assignment, some weird colors.

//To Do:
//1. Add interrupt functionality for mode changing. Currently when I try (see commented out code) the animation stops working at all, maybe because the interrupt is always triggering or I have the wrong pin or both.
  //Need to experiment a little with those things.

#define NUM_LEDS 150
#define NUM_MODES 2

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_LEDS> newColor;
const int cycles = 6;
//int interruptPin = D0; //see image in current directory (BaconSD C://Documents/Arduino/FastLEDProjects) to see Arduino's pinout mapping.

void setup() { 
  FastLED.addLeds<WS2812,3>(leds, NUM_LEDS);
  srand(time(NULL));
  FastLED.setMaxRefreshRate(100);
  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
}

void loop(){
//  scrollingRainbow();
//  stripeScroll();
//  stackLights();
//  breathing();
  randExplosions();
}

//ICACHE_RAM_ATTR void handleInterrupt(){
//  Serial.println("Interrupt handling...cycle at: ");
//  if(interruptPin == LOW){
//    cycleNumber++;
//    Serial.println(cycleNumber);
//    if(cycleNumber > NUM_MODES){
//      cycleNumber = 1;
//    }
//  }
//}

void scrollingRainbow(){
  fill_rainbow(leds, NUM_LEDS, 0, 7);
  scroll();
}

void breathing(){ //cycles between all primary and secondary colors
  int powerScale = 0;
  int increment = 1;
  bool upIncrement = true;
  int colorNumber = 0;
  int numCycles = 0;
  bool firstCycle = true;
  while(1){
    if(numCycles == cycles + 1){
      return;
    }
    CRGB color = colorSelector(colorNumber, powerScale);
    for(int b = 0; b <= NUM_LEDS; b++){
        leds[b] = color;
    }
    FastLED.show();
    //Serial.println("Value of powerScale is: ");
    //Serial.println(powerScale);
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
   
  }
}

void stripeScroll(){
  int color = 0;
  int r = 255;
  int g = 0;
  int b = 0;
  int cycles = 0;
  while(1){
       for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    leds.fadeToBlackBy(40);

    // let's set an led value

    leds[i] = CRGB(g,r,b);

    // now, let's first 20 leds to the top 20 leds, 
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.delay(33);
  }
    r-= 20;
    b+=20;
    if(r <= 0){
      r = 255;
      return;
    }
    if(g >= 255){
      g = 0;
    }
    if(b >= 255){
      b = 0;
    }
    
  }
}

void stack(bool colorMeld){
  int notLit = NUM_LEDS - 1; //accounts for odd numbers
  int currentColor = 0;
  bool stringFilled = false;
  bool firstCycle = true;
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  while(!stringFilled){
    for(int i = 0; i <= notLit / 2; i++){
      switch(currentColor){
        case 0: leds[i] = CRGB(47, 7, 39);
        break;
        case 1: leds[i] = CRGB(0, 100, 0);
        break;
        case 2: leds[i] = CRGB(0, 100, 100);
        break;
      }
        if(i-3 >= 0){
          if(i-3 < (notLit/2) - 2){ //determines how many LEDs to keep up, allowing the stack effect.
          leds[i-3] = CRGB(0, 0, 0); //turn off anything that is no longer within two spaces behind i
        }
      }
      leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
      FastLED.show();
    FastLED.delay(10);

    if(colorMeld){
      for(int j = (NUM_LEDS/2) - notLit; j <= NUM_LEDS; j++){
        
      }
    }
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
}

void stackLights(){
  int notLit = NUM_LEDS - 1; //accounts for odd numbers
  int currentColor = 0;
  bool stringFilled = false;
  bool firstCycle = true;
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
  while(!stringFilled){
    for(int i = 0; i <= notLit / 2; i++){
      switch(currentColor){
        case 0: leds[i] = CRGB(47, 7, 39);
        break;
        case 1: leds[i] = CRGB(0, 100, 0);
        break;
        case 2: leds[i] = CRGB(0, 100, 100);
        break;
      }
       //set turquoise
        if(i-3 >= 0){
          if(i-3 < (notLit/2) - 2){ //determines how many LEDs to keep up, allowing the stack effect.
          leds[i-3] = CRGB(0, 0, 0); //turn off anything that is no longer within two spaces behind i
        }
      }
      leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
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
  scroll();
} 

//void ripple(){
//  
//}


void randExplosions(){
  int numOfExplosions = 0;
  int ledPosition;
  bool doneWithSplodeys = false;
  while(!doneWithSplodeys){
    ledPosition = rand() % 150;
      explosion(10, ledPosition, colorSelector(rand() % 5, 150)); //make an explosion over the whole strip
      for(int i = 0; i < 15; i++){
        leds.fadeToBlackBy(100);
        FastLED.show();
        FastLED.delay(50); 
      }
      FastLED.delay(1000);
      numOfExplosions++;
      if(numOfExplosions == cycles){
        doneWithSplodeys = true;
      }
  }
}

void explosion(int expSize, int ledPosition, CRGB color){
  int above = ledPosition + expSize;
  int below = ledPosition - expSize;
  if(expSize % 2 == 0){
    expSize++; //make size odd so explosion can be symmetrical from a single central LED
  }
  for(int i = ledPosition; i < ledPosition + expSize; i++){
    leds.fadeToBlackBy(100);
    if(i <= NUM_LEDS && i >= 0){
      leds[i] = color;
    }
    if(above > NUM_LEDS){ //checks to make sure assignment doesn't go out of bounds. If it does, it resets the board
      leds(ledPosition,NUM_LEDS) = leds(ledPosition, ledPosition-expSize-1); //still broken. Leaves only a single LED lit
    } //and will need a check to see whether the counter should go up or down depending on if it will iterate over
      //the top or below the bottom of the array.
    else if(below < 0){
      leds(ledPosition, 0) = leds(ledPosition,ledPosition + expSize);
    }
    else{
      leds(ledPosition, ledPosition-expSize-1) = leds(ledPosition,ledPosition + expSize);
    }
    FastLED.show();
    FastLED.delay(50);
  }
}

void fill_rainbow( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue, uint8_t brightness )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = brightness % 255;
    hsv.sat = 240;
    for( int i = 0; i < numToFill; i++) {
        pFirstLED[i] = hsv;
        hsv.hue += deltahue;
    }
}

void scroll(){
  newColor = leds; //make copy of initial values
  int numCycles = 0;
  int j = 1;
  while(1){
    if(j == NUM_LEDS + 1){//if j becomes greater than the amount of LEDS, reset to 1 to take from beginning of array again
    j = 1;
    numCycles++;
    if(numCycles == 10){
      return;
    }
  }
  for(int i = 0; i <= NUM_LEDS; i++){
    int diff = i - j;
    if(diff < 0){ //if the difference is below zero, need to access element that is 'diff' less than number of leds
      leds[i] = newColor[NUM_LEDS - abs(diff)];
    }
    else{
      leds[i] = newColor[i - j]; //if the difference isn't less than 0 (EX: i = 10, j = 2), simply grab the correct color.
    }
  }
    leds(NUM_LEDS/2,NUM_LEDS-1) = leds(NUM_LEDS/2 - 1 ,0);
    FastLED.show();
    FastLED.delay(20);
    j++; //increment j to reach the next static color
  }
}

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
