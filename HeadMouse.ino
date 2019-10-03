/*Use a gyroscope to emulate a mouse and a presure
 * sensor for clicking and scrolling using a mouth 
 * piece.
 * Include some libraries
 * MPU6050 and I2Cdev from https://github.com/jrowberg/i2cdevlib
 */
 

#include <Mouse.h>
#include <I2Cdev.h>
#include <MPU6050.h>


// Define pins for debuging mouse clicks, this is just LEDs
#define leftLedPin 4
#define rightLedPin 5

//Define inttypes to the 6 axis values which will be obtained from the MPU6050
int16_t Ax, Ay, Az, Gx, Gy, Gz;
// From examples from the library
MPU6050 mpu;

//Define the values that will be sent to the Mouse
int mx;
int my;
//Define a scaler value to change the sensitivity of the mouse
int scaler = 180;



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
    else{
      Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    }

   // Smoothing of mouse movement, initialize empty arrays
   for (int i = 0; i < nReadings; i++){
    xReadings[i] = 0;
    yReadings[i] = 0;
   }

  
}

void loop() {

  // Mouse movement

  unsigned long currentMouseMillis = millis();

  if (currentMouseMillis - prevMouseMillis >= mouseInterval){
    prevMouseMillis = currentMouseMillis;
   
    mpu.getMotion6(&Ax, &Ay, &Az, &Gx, &Gy, &Gz);
    mx = -(Gz-200)/scaler; //adjust offsets for the gyro to cancel drift  
    my = (Gy-225)/scaler; 
    
    //smoothing of mouse movement
    xTot = xTot - xReadings[readIndex];
    yTot = yTot - yReadings[readIndex];

    xReadings[readIndex] = mx;
    yReadings[readIndex] = my;

    xTot = xTot + xReadings[readIndex];
    yTot = yTot + yReadings[readIndex];

    readIndex = readIndex + 1;

    //reset readIndex if nReads are reached
    if (readIndex <= nReadings){
      readIndex = 0;
    }

    xAvg = xTot / nReadings;
    yAvg = yTot / nReadings;
    
    
    Mouse.move(xAvg, yAvg);
    //Serial.print(xAvg);
    //Serial.print(" : ");
    //Serial.println(yAvg);
  }
  

  // ENd of mouse movement

  //Beginning of clicking and scrolling section

  p = analogRead(pPin);

  Serial.println(p);










  
  

}
