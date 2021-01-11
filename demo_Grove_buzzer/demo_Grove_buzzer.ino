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

    Info on the Grove device at https://wiki.seeedstudio.com/Grove-Buzzer/

    EXAMPLE NOTES
    1. Install the Grove buzzer device into slot 'D0' on the Arduino MKR Connector Carrier board
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor, the serial connection speed is set by SERIAL_SPEED at 9600 bps

    A. The code will cycle the digital between HIGH and LOW each second, the buzzer will sound one second ON, and one second OFF.
    B. The content (value) of a 'static' variable persists, for example value of 'tsStart' persists between calls to the 'timerExpired' function, the value of the variable is initialized the first time the function is called and retains its value between calls.
    C. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
    D. The timerExpired function has limited accuracy and is expected to drift over time.

    OUTPUT AT THE SERIAL PORT
    Starting buzzer..
    ............
*/

#include <Arduino.h>
#define BZR_PIN 0

bool timerExpired(unsigned long timeDelta = 1000)
{
  static unsigned long tsStart = millis(); // this value will persist, see note B above.

  unsigned long tsNow = millis();
  unsigned long tsEnd = tsStart + timeDelta;

  if (tsNow <= tsEnd)
  { // not expired
    return false;
  }
  else
  { // expired
    tsStart = tsNow;
    return true;
  }
};

void setup() {
  Serial.begin(9600);
    
  while (!Serial)
      delay(1000);

  Serial.println("");
  Serial.println("Starting buzzer..");

  pinMode(BZR_PIN, OUTPUT);
  digitalWrite(BZR_PIN, LOW);
}

void loop() {
  if (timerExpired())
  {
    digitalWrite(BZR_PIN, !digitalRead(BZR_PIN));
    Serial.print(F("."));
  };
}
