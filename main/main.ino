#include "aguro.h"
#include "sensor.h"
#include "definitions.h"

Aguro aguro;
/*
// AGURO FUNCTIONS
    aguro.init(bool debug, Sensor* x);
    aguro._start();
    aguro.setPID(P, I, D); // P: 0-255(float), I: 0-255(float), D: 0-255(float)
    aguro.followUntil(mode, speed); // mode: TJ, FR, FL, speed: 0-255
    aguro.followUntil(mode, speed, delay); // mode: TJ, FR, FL, speed: 0-255, stop delay in ms
    aguro.traceLine(speed, use_I); // speed: 0-255, use_I: true/false use Integral PID (false recomended)
    aguro.left(speed, time); // speed: 0-255, time in ms
    aguro.right(speed, time); // speed: 0-255, time in ms
    aguro.mundur(speed, time, check_switch); // speed: 0-255, time in ms, boolean check limit switch or not
    aguro.magnet(state); // state: true/false turn on/off electromagnet

// ARDUINO BUILTIN FUNCTIONS
    https://www.arduino.cc/reference/en/#functions
    FOR SHORT:
    deleay(time); // time in ms
    delayMicroseconds(time); // time in us
    digitalWrite(pin, state); // pin: 0-13, state: HIGH/LOW
    digitalRead(pin); // pin: 0-13
    analogWrite(pin, value); // pin: 3, 5, 6, 9, 10, 11, value: 0-255 // generate PWM in Nano
    analogRead(pin); // pin: 0-13
    cos(); // return cosine of angle in radians
    sin(); // return sine of angle in radians
    tan(); // return tangent of angle in radians
    randomSeed(seed); // seed: 0-65535
    random(min, max); // min: 0-255, max: 0-255
    random(max); // max: 0-255
    map(value, fromLow, fromHigh, toLow, toHigh); // value: 0-255, fromLow: 0-255, fromHigh: 0-255, toLow: 0-255, toHigh: 0-255
    constrain(value, low, high); // value: 0-255, low: 0-255, high: 0-255
    sqrt(value); // value: 0-255
    abs(value); // value: 0-255 (absolute value) MUTLAK
*/

// bool start_state = true;

void setup()
{
    Serial.begin(115200);
    pinMode(3, INPUT_PULLUP);
    delayMicroseconds(100);
    // attachInterrupt(digitalPinToInterrupt(3), myInterrupt, FALLING);

    // LED
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);

    // AGURO
    aguro.init(true);
    aguro.setPID(1.5, 0.0, 0.3); // Ku = 2 T = 0.7  Kd = 2*0.1*0.7
    aguro._start();
    aguro.magnet(false);
}

bool home = true;
void loop()
{
    int speed = 100;
    // aguro.traceLine(speed);
    // aguro.motor(75, 110);
    // aguro.mundur(100, 50000, false);
    // delay(5000);
    // aguro.stop_motor();
    // delay(4000);
    // LOOPING THROUGH THE ARENA

    if (home)
    {
        aguro.followUntil(FR, speed, 100);
        // delay(1000);
        aguro.right_auto();
        // delay(1000);
        home = !home;
    }
    for (int i = 0; i < 5; i++)
    {
        aguro.followUntil(TJ, speed, 20);
        blink_led(2, 50, CRGB::Blue);
    }
    aguro.followUntil(TJ, speed, 200);
    delay(200);
    aguro.left_auto();
    delay(2000);
    aguro.maju(100, 300);
    aguro.magnet(true);
    aguro.mundur(100, 3000, false);
    aguro.followUntil(FL, speed + 20);
    blink_led(2, 50, CRGB::Blue);
    aguro.followUntil(FR, speed + 15);
    blink_led(2, 50, CRGB::Blue);
    aguro.followUntil(FL, speed + 15);
    blink_led(2, 50, CRGB::Blue);
    aguro.followUntil(FL, speed + 15);
    blink_led(2, 50, CRGB::Blue);
    aguro.stop();
    while (1)
    {
    }
}
