/* uno rgb leds */
#include <FastLED.h>

#define LED_PIN 6
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define BRIGHTNESS 100
#define NUM_LEDS 28

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

#define BIG_RING_START 0
#define BIG_RING_SIZE 16
#define SMALL_RING_START 16
#define SMALL_RING_SIZE 12

int big_ring_red_pos = BIG_RING_START;
int small_ring_red_pos = SMALL_RING_START;

void rotate_rings(void)
{
  uint32_t current_ms;
  int i;
  
  current_ms = millis();

  /* is it time for an update? */
  if (current_ms > last_pixel_update_ms + PIXEL_UPDATE_RATE_MS)
  {
    /* Update the big ring */
    for (i = BIG_RING_START; i < BIG_RING_SIZE; i++)
    {
      if (i == big_ring_red_pos)
      {
        leds[i] = CRGB::Red;
      }
      else
      {
        leds[i] = CRGB::Blue;
      }
    }
    big_ring_red_pos++;
    if (big_ring_red_pos == BIG_RING_SIZE)
    {
      big_ring_red_pos = 0;
    }

    /* Update the small ring */
    Serial.println("=====");
    for (i = SMALL_RING_START; i < (SMALL_RING_START + SMALL_RING_SIZE); i++)
    {
      if (i == small_ring_red_pos)
      {
        leds[i] = CRGB::Red;
        Serial.print("red: ");
        Serial.println(i);
      }
      else
      {
        leds[i] = CRGB::Blue;
        Serial.print("blue: ");
        Serial.println(i);
      }
    }
    small_ring_red_pos++;
    if (small_ring_red_pos == (SMALL_RING_SIZE + SMALL_RING_START) )
    {
      small_ring_red_pos = SMALL_RING_START;
    }
    
    FastLED.show();
    last_pixel_update_ms = current_ms;
  }
      
}

void setup_pattern()
{
  //fill_gradient_RGB(leds, NUM_LEDS, CRGB::Red, CRGB::Blue);
  leds[0] = CRGB::Red;
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
  rotate_rings();

}
