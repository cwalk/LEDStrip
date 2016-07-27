#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_LEDS 30
#define PIN 6
#define BRIGHTNESS 20
#define BUTTON_PIN   2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

byte ballColors[5][3] = { {0xff, 0,0},       // red
                        {0, 0xff, 0},      // green
                        {0, 0, 0xff},      // blue
                        {0xff, 0xff, 0xff},// white 
                        {0xff, 0xff, 0} }; // yellow

bool oldState = HIGH;
int showType = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 25)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 5, 180), 50);  // Pink
            break;
    case 2: colorWipe(strip.Color(255, 0, 0), 50);  // Red
            break;
    case 3: colorWipe(strip.Color(255, 150, 0), 50);  // Orange
            break;
    case 4: colorWipe(strip.Color(255, 255, 5), 50);  // Yellow
            break;
    case 5: colorWipe(strip.Color(0, 255, 0), 50);  // Green
            break;
    case 6: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
            break;
    case 7: colorWipe(strip.Color(135, 10, 215), 50);  // Purple
            break;
    case 8: theaterChase(strip.Color(127, 127, 127), 50); // White
            break;
    case 9: theaterChase(strip.Color(127,   0,   0), 50); // Red
            break;
    case 10: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
            break;
    case 11: rainbow(20);
            break;
    case 12: rainbowCycle(20);
            break;
    case 13: theaterChaseRainbow(50);
            break;
    case 14: rainbowStrip();
            break;
    case 15: alternateColor(strip.Color(255, 0, 0), strip.Color(0, 255, 0), 100);
            break;
    case 16: randomColorFill(25);
            break;
    case 17: randomPositionFill(strip.Color(255, 255, 255), 500);
            break;
    case 18: middleFill(strip.Color(0, 255, 0), 100);
            break;
    case 19: sideFill(strip.Color(255, 0, 0), 100);
            break;
    case 20: RGBLoop();
            break;
    case 21: FadeInOut(0xff, 0x00, 0x00); // red
            break;
    case 22: FadeInOut(0xff, 0xff, 0xff); // white
            break;
    case 23: FadeInOut(0x00, 0x00, 0xff); // blue
            break;
    case 24: Strobe(0xff, 0x77, 0x00, 10, 100, 1000);
            break;
    case 25: CylonBounce(0xff, 0, 0, 3, 35, 75);
            break;
    case 26: NewKITT(0xff, 0, 0, 4, 35, 75);
            break;
    case 27: Twinkle(0xff, 0xff, 0xff, 15, 150, false);
            break;
    case 28: TwinkleRandom(20, 100, false);
            break;
    case 29: Sparkle(0xff, 0xff, 0xff, 75);
            break;
    case 30: Sparkle(random(255), random(255), random(255), 75);
            break;
    case 31: SnowSparkle(0x10, 0x10, 0x10, 20, 200);
            break;
    case 32: SnowSparkle(0x10, 0x10, 0x10, 20, random(100,1000));
            break;
    case 33: RunningLights(0xff,0,0, 50);        // red
            break;
    case 34: RunningLights(0xff,0xff,0xff, 50);  // white
            break;
    case 35: RunningLights(0,0,0xff, 50);        // blue
            break;
    case 36: Fire(55,120,20);
            break;
    case 37: BouncingBalls(0xff,0,0, 3);
            break;
    case 38: BouncingColoredBalls(5, ballColors);
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

void rainbowStrip() {
  
  int i = 0;
  while(i<NUM_LEDS*4) {     
    strip.setPixelColor(i%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+1)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+2)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+3)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+4)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+5)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    strip.setPixelColor((i+6)%NUM_LEDS, strip.Color(0, 0, 255)); //change RGB color value here
    strip.setPixelColor((i+7)%NUM_LEDS, strip.Color(135, 10, 215)); //change RGB color value here
    strip.setPixelColor((i+8)%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+9)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+10)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+11)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+12)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+13)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    strip.setPixelColor((i+14)%NUM_LEDS, strip.Color(0, 0, 255)); //change RGB color value here
    strip.setPixelColor((i+15)%NUM_LEDS, strip.Color(135, 10, 215)); //change RGB color value here
    strip.setPixelColor((i+16)%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+17)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+18)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+19)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+20)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+21)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    strip.setPixelColor((i+22)%NUM_LEDS, strip.Color(0, 0, 255)); //change RGB color value here
    strip.setPixelColor((i+23)%NUM_LEDS, strip.Color(135, 10, 215)); //change RGB color value here
    strip.setPixelColor((i+24)%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+25)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+26)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+27)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+28)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+29)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    i++;
    strip.show();
    delay(75);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// alternate between two colors on even and odd LEDs
void alternateColor(uint32_t c1, uint32_t c2, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(i%2 == 0) { // set even LED to color 1
      strip.setPixelColor(i, c1);
    } else { // set odd LED to color 2
      strip.setPixelColor(i, c2);
    }
  }

  strip.show(); // apply the colors
  delay(wait);

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    if(i%2 == 0) { // set even LED to color 2
      strip.setPixelColor(i, c2);
    } else { // set odd LED to color 1
      strip.setPixelColor(i, c1);
    }
  }

  strip.show(); // apply the colors
  delay(wait);
}

void clearStrip(){
  for(int i=0;i<NUM_LEDS;i++){
      strip.setPixelColor(i, strip.Color(0, 0, 0)); //change RGB color value here
   }
   strip.show();
}

// gradually fill up the strip with random colors
void randomColorFill(uint8_t wait) {
  clearStrip();

  for(uint16_t i=0; i<strip.numPixels(); i++) { // iterate over every LED of the strip
    int r = random(0,255); // generate a random color
    int g = random(0,255);
    int b = random(0,255);

    for(uint16_t j=0; j<strip.numPixels()-i; j++) { // iterate over every LED of the strip, that hasn't lit up yet
      strip.setPixelColor(j-1, strip.Color(0, 0, 0)); // turn previous LED off
      strip.setPixelColor(j, strip.Color(r, g, b)); // turn current LED on
      strip.show(); // apply the colors
      delay(wait);
    }
  }
}

// pick a random LED to light up until entire strip is lit
void randomPositionFill(uint32_t c, uint8_t wait) {
  clearStrip();

  int used[strip.numPixels()]; // array to keep track of lit LEDs
  int lights = 0; // counter

  for(int i = 0; i<strip.numPixels(); i++){ // fill array with 0
    used[i] = 0;
  }

  while(lights<strip.numPixels()-1) {
    int j = random(0,strip.numPixels()-1); // pick a random LED
    if(used[j] != 1){ // if LED not already lit, proceed
      strip.setPixelColor(j, c);
      used[j] = 1; // update array to remember it is lit
      lights++;
      strip.show(); // display
      delay(wait);
    }
  }
}

// Light up the strip starting from the middle
void middleFill(uint32_t c, uint8_t wait) {
  clearStrip();
  delay(wait);

  for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // start from the middle, lighting an LED on each side
    strip.setPixelColor(strip.numPixels()/2 + i, c);
    strip.setPixelColor(strip.numPixels()/2 - i, c);
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(0, c);
  strip.show();
  delay(wait);

  for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // reverse
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.setPixelColor(strip.numPixels() - i, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
  clearStrip();
}

// Light up the strip starting from the sides
void sideFill(uint32_t c, uint8_t wait) {
  clearStrip();
  delay(wait);

  for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // fill strip from sides to middle
    strip.setPixelColor(i, c);
    strip.setPixelColor(strip.numPixels() - i, c);
    strip.show();
    delay(wait);
  }

  strip.setPixelColor(strip.numPixels()/2, c);
  strip.show();
  delay(wait);

  for(uint16_t i=0; i<(strip.numPixels()/2); i++) { // reverse
    strip.setPixelColor(strip.numPixels()/2 + i, strip.Color(0, 0, 0));
    strip.setPixelColor(strip.numPixels()/2 - i, strip.Color(0, 0, 0));
    strip.show();
    delay(wait);
  }
  clearStrip();
}

/*Extra stuff*/
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}

void RGBLoop(){
  for(int j = 0; j < 3; j++ ) { 
    // Fade IN
    for(int k = 0; k < 256; k++) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for(int k = 255; k >= 0; k--) { 
      switch(j) { 
        case 0: setAll(k,0,0); break;
        case 1: setAll(0,k,0); break;
        case 2: setAll(0,0,k); break;
      }
      showStrip();
      delay(3);
    }
  }
}

void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
      
  for(int k = 0; k < 256; k=k+1) { 
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
     
  for(int k = 255; k >= 0; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    setAll(r,g,b);
    showStrip();
  }
}

void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}

void NewKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

void CenterToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i =((NUM_LEDS-EyeSize)/2); i>=0; i--) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void OutsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i<=((NUM_LEDS-EyeSize)/2); i++) {
    setAll(0,0,0);
    
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    
    setPixel(NUM_LEDS-i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS-i-j, red, green, blue); 
    }
    setPixel(NUM_LEDS-i-EyeSize-1, red/10, green/10, blue/10);
    
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void LeftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void RightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay) {
  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
  
  for (int i=0; i<Count; i++) {
     setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) { 
       setAll(0,0,0); 
     }
   }
  
  delay(SpeedDelay);
}

void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}

void SnowSparkle(byte red, byte green, byte blue, int SparkleDelay, int SpeedDelay) {
  setAll(red,green,blue);
  
  int Pixel = random(NUM_LEDS);
  setPixel(Pixel,0xff,0xff,0xff);
  showStrip();
  delay(SparkleDelay);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  
  for(int i=0; i<NUM_LEDS*2; i++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
      
      showStrip();
      delay(WaveDelay);
  }
}

void colorWipe2(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
  
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
    
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
  
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
    
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void BouncingBalls(byte red, byte green, byte blue, int BallCount) {
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],red,green,blue);
    }
    
    showStrip();
    setAll(0,0,0);
  }
}

void BouncingColoredBalls(int BallCount, byte colors[][3]) {
  float Gravity = -9.81;
  int StartHeight = 1;
  
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
  
  for (int i = 0 ; i < BallCount ; i++) {   
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0; 
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2); 
  }

  while (true) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
  
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
  
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }
  
    for (int i = 0 ; i < BallCount ; i++) {
      setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    }
    
    showStrip();
    setAll(0,0,0);
  }
}
