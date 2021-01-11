/*  Copyright (c) 2020 SORACOM
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
    
    Dependent Modules
    Arduino : by Arduino LLC, subject to the GNU Lesser General Public License.  This library is available in the Arduino IDE and also at https://github.com/arduino/Arduino
    ADXL345 : by seeed technology inc., subject to the MIT License (MIT).   This library is available in the Arduino IDE and also at https://github.com/Seeed-Studio/Accelerometer_ADXL345
    Wire : by Arduino LLC, subject to the GNU Lesser General Public License.  This library is available in the Arduino IDE

    Info on the Grove device at;
    https://github.com/Seeed-Studio/Accelerometer_ADXL345

    EXAMPLE NOTES
    1. Install the ADXL345 device into slot TWI on the Arduino MKR Connector Carrier board
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor, the serial connection speed is set by SERIAL_SPEED at 9600 bps

    A. The ADXL345 object will emit readings in the X, Y & Z axis, moving & rotating the device will change the readings
    B. The content (value) of a 'static' variable persists, for example value of 'tsStart' persists between calls to the 'timerExpired' function, the value of the variable is initialized the first time the function is called and retains its value between calls.
    C. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
    D. The timerExpired function has limited accuracy and is expected to drift over time.

    OUTPUT AT THE SERIAL PORT
    ADXL345 starting..
    Force X:142 Y:141 Z:-129
    Force X:142 Y:143 Z:-129
    Force X:142 Y:141 Z:-128
    Force X:142 Y:142 Z:-127
    Force X:142 Y:144 Z:-126
    Acceleration X:0.5344738000 Y:0.5414529600 Z:-0.4400739000
    Force X:35 Y:33 Z:-223
*/
#include <Arduino.h>

#include <Wire.h>
#include <ADXL345.h>

ADXL345 sensor;

bool timerExpired(unsigned long timeDelta = 1000)
{
  static unsigned long tsStart = millis(); // this value will persist, see note B above.

  unsigned long tsNow = millis();
  unsigned long tsEnd = tsStart + timeDelta;

  if(tsNow <= tsEnd)
  { // not expired
    return false;
  }
  else
  { // expired
  tsStart = tsNow;
  return true;
  }
};

void setup()
{
  Serial.begin(9600);

  while (!Serial)
      delay(1000); // remove this while loop to start the sketch at power-on, else the sketch will wait for a serial connection

  Serial.println(F(""));
  Serial.println(F("ADXL345 starting.."));

  sensor.powerOn(); // using the default I2C (TWI) pins on the MKR
                    // SDA on pin 11 & 
                    // SCL on pin 12
}

void loop()
{
  static unsigned int loopCounter = 0;  // this value will persist, see note B above.
  
  if (timerExpired())
  {
    int x, y, z;

    loopCounter = loopCounter + 1;

    sensor.readXYZ(&x, &y, &z);

    Serial.print(F("Force X:"));
    Serial.print(x, DEC);

    Serial.print(F(" Y:"));
    Serial.print(y, DEC);

    Serial.print(F(" Z:"));
    Serial.println(z, DEC);
  };

  if (loopCounter == 5)
  {
    double acceleration[3];

    loopCounter = 0;

    sensor.getAcceleration(acceleration);

    Serial.print(F("Acceleration X:"));
    Serial.print

    (acceleration[0], DEC);

    Serial.print(F(" Y:"));
    Serial.print(acceleration[1], DEC);

    Serial.print(F(" Z:"));
    Serial.println(acceleration[2], DEC);
  };
}
