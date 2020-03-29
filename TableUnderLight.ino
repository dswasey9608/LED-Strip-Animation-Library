#include <FastLED.h> 

//NOTE: CRGB assignment functions as CGRB for some reason.
//Check 'addedUtils.cpp' to find functions used in this sketch

//Scrolling Demo Working... Uses literal D3 pin on board, WS2812 working for assignment, some weird colors.

//To Do:
//1. Add interrupt functionality for mode changing. Currently when I try (see commented out code) the animation stops working at all, maybe because the interrupt is always triggering or I have the wrong pin or both.
  //Need to experiment a little with those things.

#define NUM_LEDS 150

CRGBArray<NUM_LEDS> leds;
CRGBArray<NUM_LEDS> newColor;
const int cycles = 2;
bool red = true;
int count = 0;
CRGB colorArray[3] = {CRGB(0,150,0), CRGB(0,0,150), CRGB(150,0,0)};
int button = 5; //D1
addedUtils modder(leds, NUM_LEDS, cycles);
//int interruptPin = D0; //see image in current directory (BaconSD C://Documents/Arduino/FastLEDProjects) to see Arduino's pinout mapping.

void setup() { 

  Serial.begin(115200);
  FastLED.addLeds<WS2812,3>(leds, NUM_LEDS);
  srand(time(NULL));
  FastLED.setMaxRefreshRate(100);
  leds.fill_solid(CRGB(0,150,0));
  FastLED.show(); 
  pinMode(button, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
  
}

void loop(){
//  scrollingRainbow(); //too distracting for internal lights
//  stripeScroll();
//  modder.stackLights(CRGB(0, 100, 0), CRGB(0, 0, 100), CRGB(100, 100, 100));//Captain America
//  modder.breathing(true);
//  modder.stackLights(CRGB(100, 0, 0), CRGB(0, 100, 100), CRGB(100, 100, 100)); //Hulk
//  modder.breathing(true);
//  modder.stackLights(CRGB(20, 162, 26), CRGB(75, 64, 86), CRGB(87, 98, 50)); //
//  modder.breathing(true);
//  modder.stackLights(CRGB(0, 100, 0), CRGB(75, 75, 75), CRGB(100, 100, 0)); //Iron Man
//  modder.breathing(true);
//  modder.breathing(false); //cycle through my colors
//  randExplosions();
//  modder.breathSelect(CRGB(150, 0, 0), CRGB(0, 150, 0), CRGB(0, 0, 150));
//  modder.stackLights(CRGB(0, 100, 0), CRGB(0, 0, 100), CRGB(100, 100, 100));
//  for(int i = 0; i < 100; i++){
//    leds.fadeToBlackBy(25);
//    FastLED.delay(20);
//  }
//  modder.stackLights(CRGB(0, 100, 0), CRGB(75, 75, 75), CRGB(100, 100, 0));
//  for(int i = 0; i < 100; i++){
//    leds.fadeToBlackBy(25);
//    FastLED.delay(20);
//  }
//  stripeScroll();
//  scrollingRainbow();
//  for(int i = 0; i < 100; i++){
//    leds.fadeToBlackBy(25);
//    FastLED.delay(20);
//  }
  if(digitalRead(button) == 0){
//    if(red){
//      gradient_transition(colorArray[0],colorArray[1]); 
//      red = false;
//    }
//    else{ 
//      gradient_transition(colorArray[1],colorArray[0]); 
//      red = true;
//    }
    switch(count){
      case 0: gradient_transition(colorArray[0],colorArray[1]);
              count++;
              break;
      case 1: gradient_transition(colorArray[1],colorArray[2]);
              count++;
              break;
      case 2: gradient_transition(colorArray[2],colorArray[0]);
              count = 0;
              break;
    }
      
    while(digitalRead(button) == 0){delay(10);}
  }
  FastLED.show();
  
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
  modder.scroll(cycles);
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
void randExplosions(){
  int numOfExplosions = 0;
  int ledPosition;
  bool doneWithSplodeys = false;
  while(!doneWithSplodeys){
    ledPosition = rand() % 150;
      explosion(10, ledPosition, modder.colorSelector(rand() % 5, 150)); //make an explosion over the whole strip
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

void gradient_transition(struct CRGB firstColor, struct CRGB secondColor){
  Serial.println("Starting gradient---");
  float startR = firstColor.r;
  float startG = firstColor.g;
  float startB = firstColor.b;
  float stepDistanceR = (firstColor.r - secondColor.r)/100.0;
  float stepDistanceG = (firstColor.g - secondColor.g)/100.0;
  float stepDistanceB = (firstColor.b - secondColor.b)/100.0;
  
  for(int i = 0; i<100; i++){
    startG-= stepDistanceG;
    startR-= stepDistanceR;
    startB-= stepDistanceB;
    Serial.print("Red value: ");
    Serial.print(startG);
    Serial.print(" Green value: ");
    Serial.print(startR);
    Serial.print(" Blue value: ");
    Serial.println(startB);
    leds.fill_solid(CRGB((int) startR, (int)startG, (int) startB));
    FastLED.show();
    delay(20);
  }
  leds.fill_solid(secondColor);
  FastLED.show();
}
