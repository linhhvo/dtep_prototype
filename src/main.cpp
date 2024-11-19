#include <FastLED.h>
#include <OneButton.h>
#include <hp_BH1750.h>

#define NUM_LEDS  35
#define LED_PIN  13
#define SENS_PIN 2
#define BUTTON_PIN 3

CRGB leds[NUM_LEDS];

OneButton btn;
hp_BH1750 BH1750;

int buttonState = 0;
int motionState = 0;
int ledBrightness = 10;

long startIdleTime;
long currentIdleTime;
long allowedIdleTime = 900000;


void handleClick() {
    if (buttonState == 0) {
        buttonState = 1;
    } else {
        buttonState = 0;
        motionState = 0;
    };
}

void setup() {
    pinMode(SENS_PIN, INPUT);

    btn.setup(BUTTON_PIN, INPUT_PULLUP, true);
    btn.attachClick(handleClick);

    CFastLED::addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
    // FastLED.setBrightness(ledBrightness);

    BH1750.begin(BH1750_TO_GROUND);

    Serial.begin(9600);

}

void loop() {
    btn.tick();

    motionState = digitalRead(SENS_PIN);

    if (buttonState == 1) {
        if (motionState == 1) {
            startIdleTime = millis();

            BH1750.start();   //starts a measurement
            const float lux = BH1750.getLux();
            ledBrightness = min(max(255 - (lux+10), 5), 255);

            FastLED.setBrightness(ledBrightness);

            fill_solid(leds, NUM_LEDS, CRGB::White);
            FastLED.show();


            // } else {
            //   fill_solid(leds, NUM_LEDS, CRGB::Black);
            //   FastLED.show();
        };
        currentIdleTime = millis() - startIdleTime;

        // Serial.print("timer ");
        // Serial.println(currentIdleTime);

        if ((currentIdleTime >= allowedIdleTime)  & (motionState == 0)){
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
        }

    } else {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
    };

}










