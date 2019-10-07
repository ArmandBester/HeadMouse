/*Use a gyroscope to emulate mouse movement and a presure
   sensor for clicking and scrolling using a mouth
   piece.
   Include some libraries
   MPU6050 and I2Cdev from https://github.com/jrowberg/i2cdevlib or forked at https://github.com/ArmandBester/i2cdevlib (probably best to stick with the original rather)
*/


#include <Mouse.h>
#include <I2Cdev.h>
#include <MPU6050.h>


//Define inttypes to the 6 axis values which will be obtained from the MPU6050
int16_t Ax, Ay, Az, Gx, Gy, Gz;
// From examples from the library
MPU6050 mpu;

//Define the values that will be sent to the Mouse
int mx;
int my;
//Define a scaler value to change the sensitivity of the mouse
int scaler = 100;



//Smoothing of mouse movement
const int nReadings = 3;
int xReadings[nReadings];
int yReadings[nReadings];
int xTot;
int yTot;
int xAvg;
int yAvg;
int readIndex;


//Timing control
//----> Mouse movement
unsigned long prevMouseMillis = 0;
const long mouseInterval = 10;


//Pressure click and scroll
#define pPin A0
int p = 0;
int pTimerPos = 0;
int pTimerNeg = 0;
int pThresh = 150;
int clickTrigger = 300;
int scrollTrigger = 800;
int scrollDist = 10;


// Define pins for debuging mouse clicks, this is just LEDs
#define leftLedPin 4
#define rightLedPin 5

// On startup callebrate ambient pressure
int cPressure = 0;
int aPressure = 500;  //will update this value with cPressure


void setup() {

  Serial.begin(9600); //mostly for debugging
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  pinMode(pPin, INPUT);

  // initialize the mpu
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
  }
  else {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
  }

  // Smoothing of mouse movement, initialize empty arrays
  for (int i = 0; i < nReadings; i++) {
    xReadings[i] = 0;
    yReadings[i] = 0;
  }

  //callibrate to ambient pressure

  for (int k = 0; k < 10; k++) {
    cPressure = cPressure + analogRead(pPin);
  }
  aPressure = cPressure/10;

  //end of setup

}



void loop() {

  // Mouse movement

  unsigned long currentMouseMillis = millis();

  if (currentMouseMillis - prevMouseMillis >= mouseInterval) {
    prevMouseMillis = currentMouseMillis;

    mpu.getMotion6(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
    mx = -(Gz - 200) / scaler; //adjust offsets for the gyro to cancel drift
    my = (Gy - 225) / scaler;

    //smoothing of mouse movement
    xTot = xTot - xReadings[readIndex];
    yTot = yTot - yReadings[readIndex];

    xReadings[readIndex] = mx;
    yReadings[readIndex] = my;

    xTot = xTot + xReadings[readIndex];
    yTot = yTot + yReadings[readIndex];

    readIndex = readIndex + 1;

    //reset readIndex if nReads are reached
    if (readIndex <= nReadings) {
      readIndex = 0;
    }

    xAvg = xTot / nReadings;
    yAvg = yTot / nReadings;


    Mouse.move(xAvg, yAvg);
    Serial.print(xAvg);
    Serial.print(" : ");
    Serial.println(yAvg);
  }


  // ENd of mouse movement
  //------------------------------------------------------------------------------//

  //Beginning of clicking and scrolling section

  //positive pressure controls
  if (analogRead(pPin) >= (aPressure + pThresh)){
    pTimerPos ++;
  }
  //left click
  if (analogRead(pPin) < (aPressure + pThresh) && pTimerPos >= clickTrigger && pTimerPos < scrollTrigger){
    Mouse.click();
    pTimerPos = 0;
  }
  //down scroll
  if (analogRead(pPin) < (aPressure + pThresh) && pTimerPos >= scrollTrigger){
    Mouse.move(0, 0, -scrollDist);
    pTimerPos = 0;
  }

  //negative pressure controls
  if (analogRead(pPin) <= (aPressure - pThresh)){
    pTimerNeg ++;
  }
  //right click
  if (analogRead(pPin) > (aPressure - pThresh) && pTimerNeg >= clickTrigger && pTimerNeg < scrollTrigger){
    Mouse.click(MOUSE_RIGHT);
    pTimerNeg = 0;
  }
  //up scroll
  if (analogRead(pPin) > (aPressure - pThresh) && pTimerNeg >= scrollTrigger){
    Mouse.move(0, 0, scrollDist);
    pTimerNeg = 0;
  }

  

  













}
