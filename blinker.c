#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>

const int grnLed = 23;
const int redPin = 18;
const int butPin = 17;

const int pwmValue = 75;

void *blink(void *vargp);
void *detect(void *vargp);
void switchLed(int pin, int val);

typedef struct {
    int inPin;
    int outPin;
    void (*callback)(int pin, int val);
} DetectParams;

typedef struct {
    int pin;
    int delayMs;
} BlinkParams;

int main(void) {
    wiringPiSetupGpio();

    BlinkParams bParams;
    bParams.pin = redPin;
    bParams.delayMs = 500;

    DetectParams params;
    params.inPin = butPin;
    params.outPin = grnLed;
    params.callback = switchLed;

    pthread_t tid1;
    pthread_t tid2;
    pthread_create(&tid1, NULL, detect, &params);
    pthread_create(&tid2, NULL, blink, &bParams);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}

void *blink(void *vargp) {

    BlinkParams params = *((BlinkParams *)vargp);
    int pin = params.pin;
    int delayMs = params.delayMs;

    pinMode(pin, OUTPUT);
    while(1) {
        digitalWrite(pin, HIGH);
        delay(delayMs);
        digitalWrite(pin, LOW);
        delay(delayMs);
    }
    return;
}

void *detect(void *vargp) {
    DetectParams params = *((DetectParams *)vargp);

    int val;
    int prevVal = 1; // off state
    pinMode(params.inPin, INPUT);
    pullUpDnControl(params.inPin, PUD_UP);
    pinMode(params.outPin, OUTPUT);
    while(1) {
        delay(100);
        val = digitalRead(params.inPin);
        if (val != prevVal) {
            params.callback(params.outPin, val * -1 + 1);
            prevVal = val;
        }
    }
}

void switchLed(int pin, int val) {
    digitalWrite(pin, val ? HIGH : LOW);
}
