# HeadMouse

### An Arduino based human interface device (HID)
This device uses an [Arduino Micro](https://store.arduino.cc/arduino-micro) and will probably work with any ATmega32U4 based development board.  A [gyroscope sensor](https://www.amazon.com/Gy-521-MPU-6050-MPU6050-Sensors-Accelerometer/dp/B008BOPN40)) is used as input.  The gyroscope input is smoothed and scaled by the arduino and using the [Mouse library](https://www.arduino.cc/reference/en/language/functions/usb/mouse/) is able to emulate mouse movements on a computer screen.  A pressure sensor XGZP6847 is used as input for click and scroll control.  

* Short blow = left click
* Longer blow = scroll down
* Short suck = right click
* Longer suck = scroll up

When the device is powered on via a USB cable, it automatically calibrates the ambient pressure.

The HeadMouse can be mounted on a gaming headset, this allows a good scaffold for the electronics as well as the microphone extension to which a mouthpiece is fixed to enable convenient pressure control.
