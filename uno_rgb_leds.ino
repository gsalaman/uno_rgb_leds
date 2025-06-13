/* uno rgb leds */
#include <FastLED.h>

#define LED_PIN 6
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
#define NUM_LEDS 8

CRGB leds[NUM_LEDS];



uint32_t last_pixel_update_ms = 0;
#define PIXEL_UPDATE_RATE_MS 100

void rotate_pixels(void)
{
  uint32_t current_ms;
  CRGB rollover_pixel;
  int i;
  
  current_ms = millis();

  /* is it time for an update? */
  if (current_ms > last_pixel_update_ms + PIXEL_UPDATE_RATE_MS)
  {
    /* shift all pixels by one */
    rollover_pixel = leds[0];
    for (i=0; i<NUM_LEDS-1; i++)
    {
      leds[i] = leds[i+1];
    }
    leds[NUM_LEDS - 1] = rollover_pixel;
    FastLED.show();
    last_pixel_update_ms = current_ms;
  }
}

void setup_pattern()
{
  fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Blue);
  FastLED.show();
  
}

void setup(void)
{
  int i;
  
  Serial.begin(9600);
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.println("Initialized...starting power-up-test");
  
  // Power up test...cycle all LEDs blue.
  for (i=0;i<NUM_LEDS;i++)
  {
    leds[i] = CRGB::Blue;
    FastLED.show();
    FastLED.delay(100);
  }

  Serial.println("power up test complete");

  delay(1000);
  setup_pattern();

}

void loop(void)
{
  rotate_pixels();

}
