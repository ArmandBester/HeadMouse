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
int xReadings[10];
int yReadings[10];
int xTot;
int yTot;
int xAvg;
int yAvg;
int readIndex;


//Timing control
//----> Mouse movement
unsigned long prevMouseMillis = 0;
const long mouseInterval = 20;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //mostly for debugging
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);

  // initialize the mpu
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1);
    }
    else{
      Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
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
    Mouse.move(mx, my);
    Serial.print(mx);
    Serial.print(" : ");
    Serial.println(my);
  }
  

  // ENd of mouse movement
  

}
