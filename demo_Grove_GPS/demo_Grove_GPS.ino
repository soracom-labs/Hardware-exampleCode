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

    Info on the Grove device at https://github.com/Seeed-Studio/...
    SIM28
    
    EXAMPLE NOTES
    1. Install the Grove GPS device on to the SERIAL slot on the Arduino MKR Connector Carrier board
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor, the serial connection speed is set by SERIAL_SPEED at 9600 bps
    
    A. The GPS device attached through Serial1 will emit data every second.
      $GPGSA  : Used to represent the ID’s of satellites which are used for position fix. 
      $GPGSV  : Satellite information about elevation, azimuth and CNR 
      $GPRMC  : Time, date, position, course and speed data. 
      $GPGGA  : Time, position, and fix related data of the receiver
    B. The content (value) of a 'static' variable persists, for example value of 'tsStart' persists between calls to the 'timerExpired' function, the value of the variable is initialized the first time the function is called and retains its value between calls.
    C. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
     
    OUTPUT AT THE SERIAL PORT
    Starting SIM28 test..
    $GPGGA,000324.799,,,,,0,0,,,M,,M,,*4A
    $GPGSA,A,1,,,,,,,,,,,,,,,*1E
    $GPGSV,1,1,00*79
    $GPRMC,000324.799,V,,,,,0.00,0.00,060180,,,N*40
*/

#include <Arduino.h>

#define BUFSIZE 64

unsigned char buffer[BUFSIZE];
int count = 0;

void setup()
{
  Serial1.begin(9600);
  Serial.begin(9600);

  while (!Serial)
      delay(1000); // remove this while loop to start the sketch at power-on, else the sketch will wait for a serial connection

  Serial.println(F(""));
  Serial.println(F("Starting SIM28 test.."));
}

void loop()
{ 
  if (Serial1.available())
  {
    while (Serial1.available())
    {
      buffer[count++] = Serial1.read();
      if (count == BUFSIZE)
        break;
    }

    Serial.write(buffer, count);
    
    buffer[0] = '\n';
    count = 0;
  }
}
