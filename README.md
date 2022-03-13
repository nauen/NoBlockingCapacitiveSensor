# NoBlockingCapacitiveSensor
Capacitive Sensoring with interrupts non blocking

This sketch based on CapacitiveSense.h - Capacitive Sensing Library Copyright (c) 2009 Paul Bagder
https://github.com/PaulStoffregen/CapacitiveSensor

I added also from nhatuan84 Arduino-KalmanFilter 
https://github.com/nhatuan84/Arduino-KalmanFilter

This sketch is tested on Teensy 4.0 and Arduino Nano. It has a nice range until 1m with 30MOhm. The graph is a bit unpredictable and the spikes very sharp. Didn't find a perfect filter yet.

On Arduino Nano, the sender PIN D5 will not be used. I don't know why it's working, but it's working.

Maybe the sketch is able to run on ESP32 if it's possible to attach an interrupt. Not tryed yet.

Hints for ESP8266:

I was not able to make it run on ESP12 / ESP8266. 
It was not able to get Values on Interrupt PIN. Maybe here are some hints: https://blog.hiebl.cc/posts/wemos-d1-mini-pro-digital-interrupt-pins/

```
/*
 * 
#define D0  16
#define D1  5  // I2C Bus SCL (clock)
#define D2  4  // I2C Bus SDA (data)
#define D3  0
#define D4  2  // Same as "LED_BUILTIN", but inverted logic
#define D5  14 // SPI Bus SCK (clock)
#define D6  12 // SPI Bus MISO 
#define D7  13 // SPI Bus MOSI
#define D8  15 // SPI Bus SS (CS)
#define D9  3  // RX0 (Serial console)
#define D10 1  // TX0 (Serial console)
 */

// geht 1 CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);            // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht 2 CapacitiveSensor   cs_4_2 = CapacitiveSensor(0,2);            // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht 3CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,2);            // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,4);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,5);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,0);             // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,14);      // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,12);      // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht       CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,13);      // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// gehtt nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,15);      // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(15,1);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
// geht nicht CapacitiveSensor   cs_4_2 = CapacitiveSensor(2,15);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired

ICACHE_RAM_ATTR void interruptReceive() {
  cs_4_2.total += micros() - cs_4_2.totalTimer;
  if (cs_4_2.cycle > 0) {
    cs_4_2.SenseOneCycle();
    cs_4_2.cycle--;
  }
  else {
    // routine to subtract baseline (non-sensed capacitance) from sensor return
    if (cs_4_2.total < cs_4_2.leastTotal)
      cs_4_2.leastTotal = cs_4_2.total;                 // set floor value to subtract from sensed value
    Serial.print(20);
    Serial.print("\t");                    // tab character for debug windown spacing
    Serial.print(200);
    Serial.print("\t");                    // tab character for debug windown spacing
    //Serial.println(kalmanFilter((cs_4_2.total - cs_4_2.leastTotal)));
    Serial.println(cs_4_2.total - cs_4_2.leastTotal);
  }
}

.
.
.
```
