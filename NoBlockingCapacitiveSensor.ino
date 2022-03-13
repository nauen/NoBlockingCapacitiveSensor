#include "CapacitiveSensor.h"
#include "ArduiKalman.h"

CapacitiveSensor   cs_4_2 = CapacitiveSensor(2, 3);       // 10M resistor between pins 4 & 2, pin 2 is sensor pin, add a wire and or foil if desired
float touchValue;
float touchValueTime;
byte cycles = 0;
byte touchDepth = 10;
float touchValues[10];
long interruptsCounter, interruptCounterOld, interruptCounterNew = 0;
byte touchValueIndex = 0;
long blubTimer;
int softer[100];

int softIndex = 0;
int soft = 0;

int touches[1000];
int touchIndex = 0;
int touche = 0;
void interruptReceive() {
  interruptsCounter++;
  cs_4_2.total += micros() - cs_4_2.totalTimer;
  if (cs_4_2.cycle > 0) {
    cs_4_2.SenseOneCycleNoBlock();
    cs_4_2.cycle--;
  }
  else {

    // routine to subtract baseline (non-sensed capacitance) from sensor return
    if (cs_4_2.total < cs_4_2.leastTotal)
      cs_4_2.leastTotal = cs_4_2.total;                 // set floor value to subtract from sensed value

    touchValue = (cs_4_2.total - cs_4_2.leastTotal);

    if (softIndex > 9) touchIndex = 0;
    else softIndex++;
    softer[softIndex] = touchValueTime;
    soft = 0;
    for (int i = 0; i < 10; i++) {
      soft += softer[i];
    }

    if (touchIndex > 9) touchIndex = 0;
    touches[touchIndex] = touchValue;
    touche = 0;
    for (int i = 0; i < 10; i++) {
      touche += touches[i];
    }
  }

}

//init matrix for Kalman
float xc[1];        // correct state vector
float xp[1];        // predict state vector
float A[1][1];      // prediction error covariance
float Q[1][1];      // process noise covariance
float R[1][1];      // measurement error covariance
float H[1][1];      // Measurement model
float P[1][1];      // Post-prediction, pre-update

int stateNum = 1;
int measureNum = 1;
KalmanFilter m_kf;


float kalmanFilter(float total1) {
  // predict
  float *predict = m_kf.predict();
  // correct
  float rand_noise = random(-100, 100) / 100.0;
  float measured_value = total1 + rand_noise;
  float measurement[measureNum];
  measurement[0] = measured_value;
  float *correct = m_kf.correct(measurement);
  float estimated_value = correct[0];
  return estimated_value;
}

void setup() {
  m_kf.init(stateNum, measureNum, &A[0][0], &P[0][0], &Q[0][0], &H[0][0], &R[0][0], &xp[0], &xc[0]);
  m_kf.zeros();
  A[0][0] = 2.0f;
  H[0][0] = 1.0f;
  Q[0][0] = 0.01f;
  R[0][0] = 100.0f;
  P[0][0] = 1.0f;

  xc[0] = 40;

  attachInterrupt(digitalPinToInterrupt(3), interruptReceive, RISING);
}

int touchCycles = 50;

bool touchDebug = true;
void loop() {
  if (touchDebug)
    Serial.println(kalmanFilter(touchValueTime));

  touchValueTime = interruptsCounter - interruptCounterOld;
  interruptCounterOld = interruptsCounter;
  //fscale(20000, 6200000, 0, 1024 , touchValue, 0)
  //Serial.println((touchValueTime));
  //Serial.println(soft);
  /*
      Serial.print(0);
      Serial.print("\t");                    // tab character for debug windown spacing
      Serial.print(7000);
      Serial.print("\t");                    // tab character for debug windown spacing
  */
  if (cs_4_2.cycle <= 0) {
    cs_4_2.cycle = touchCycles;
    cs_4_2.capacitiveSensorNoBlock();
  }

}
