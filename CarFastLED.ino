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

int i = -1;
int j = 1;
int cycleNumber = 1;
//int interruptPin = D0; //see image in current directory (BaconSD C://Documents/Arduino/FastLEDProjects) to see Arduino's pinout mapping.

void setup() { 
  FastLED.addLeds<WS2812,3>(leds, NUM_LEDS);
  Serial.begin(115200);
  //pinMode(interruptPin, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
}

void loop(){
  scrollingRainbow();
  breathing();
  stripeScroll();
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
  int cycles = 0;
  while(1){
    if(j == NUM_LEDS + 1){//if j becomes greater than the amount of LEDS, reset to 1 to take from beginning of array again
    j = 1;
    cycles++;
    if(cycles == 10){
      return;
    }
  }
  if(i == -1){
      fill_rainbow( leds, NUM_LEDS, 0, 7);
      newColor = leds; //make copy of initial values
    }
  for(i = 0; i <= NUM_LEDS; i++){
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
    i = 0; //reset i because of it's global int properties.
  }
}

void breathing(){ //cycles between all primary and secondary colors
  int powerScale = 0;
  int increment = 1;
  bool upIncrement = true;
  int colorNumber = 0;
  int cycles = 0;
  while(1){
    for(int b = 0; b <= NUM_LEDS; b++){
      switch(colorNumber){
        case 0: leds[b] = CRGB(0,150*(powerScale*.01),0);//red
                break;
        case 1: leds[b] = CRGB(150*(powerScale*.01),150*(powerScale*.01),0);
                break;
        case 2: leds[b] = CRGB(150*(powerScale*.01),0,0);//green
                break;
        case 3: leds[b] = CRGB(150*(powerScale*.01),0,150*(powerScale*.01));
                break;
        case 4: leds[b] = CRGB(0,0,150*(powerScale*.01));//blue
                break;
        case 5: leds[b] = CRGB(0,150*(powerScale*.01),150*(powerScale*.01));
                break;
        default: leds[b] = CRGB(0,0,150*(powerScale*.01));
      }
      // //red
      // //green
       //blue
      //leds[b] = CRGB(127, (int)255*(powerScale*.02), (int)212*(powerScale*.02)); 
    }
    
    FastLED.show();
    //Serial.println("Value of powerScale is: ");
    //Serial.println(powerScale);
    if(powerScale == 100){ //float comparison picks up random values and can never find an equivalence.
      FastLED.delay(100);
      upIncrement = false;
    }
    if(powerScale == 0){
      colorNumber++;
      if(colorNumber >= 6){
        colorNumber = 0;
      }
      FastLED.delay(200);
      upIncrement = true;
      cycles++;
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

    if(cycles == 10){
      return;
    }
   
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
    r-= 10;
    b+=10;
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
