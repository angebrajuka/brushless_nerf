#include "settings.h"

#define sleep delay

void rev();
void idle();
void cycle(int);
void burst(int);

void setup() {
    pinMode(PIN_WHEEL_TOP,      OUTPUT);
    pinMode(PIN_WHEEL_BTM,      OUTPUT);
    pinMode(PIN_PUSHER,         OUTPUT);

    pinMode(PIN_SAFETY,         INPUT);
    pinMode(PIN_FPS_CONTROL,    INPUT);
}

void loop() {
    if(digitalRead(PIN_SAFETY) != HIGH && digitalRead(PIN_TRIGGER) == HIGH) {
        if(analogRead(PIN_SELECT) > ANALOG_AUTO) {
            rev();
            sleep(REV_TIME_MS);
            do {
                cycle(ROF_DPS_AUTO);
            } while(digitalRead(PIN_TRIGGER) == HIGH);
            idle();
        } else if(analogRead(PIN_SELECT) > ANALOG_SEMI) {
            burst(BURST_COUNT);
        } else {
            burst(1);
        }
    }
    idle();
}

void rev() {
    int power = digitalRead(PIN_FPS_CONTROL) == HIGH ? POWER_FPS_MAX : POWER_FPS_LOW;
    analogWrite(PIN_WHEEL_TOP, power);
    analogWrite(PIN_WHEEL_BTM, power);
}

void idle() {
    analogWrite(PIN_WHEEL_TOP, 0);
    analogWrite(PIN_WHEEL_BTM, 0);
    digitalWrite(PIN_PUSHER, LOW);
}

void cycle(int darts_per_second) {
    const int half_cycle_delay_millis = 500 / darts_per_second;
    digitalWrite(PIN_PUSHER, HIGH);
    sleep(half_cycle_delay_millis);
    digitalWrite(PIN_PUSHER, LOW);
    sleep(half_cycle_delay_millis);
}

void burst(int num) {
    rev();
    sleep(REV_TIME_MS);
    int i;
    for(i = 0; i < num; ++i) {
        cycle(ROF_DPS_BURST);
    }
    idle();
    while(digitalRead(PIN_TRIGGER) == HIGH);
}
