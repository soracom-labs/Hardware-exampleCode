
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
    Bounce2 : by thomasfredericks, subject to the MIT License (MIT).  This library is available in the Arduino IDE and also at https://github.com/thomasfredericks/Bounce2

    EXAMPLE NOTES
    1. Install the Grove reed switch device into slot 'D2' on the Arduino MKR Connector Carrier board
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor and present a magnet to the reed switch, the serial connection speed is set by SERIAL_SPEED at 9600 bps

    A. The Button2 class will detect two transitions for each pass of the magnet; the first when the magnet arrives and the second when it is removed.  Additional features are implemented in the Button2 class and are illustrated in the examples folder.
    B. The content (value) of a 'static' variable persists, for example value of 'tsStart' persists between calls to the 'timerExpired' function, the value of the variable is initialized the first time the function is called and retains its value between calls.
    C. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
    D. The timerExpired function has limited accuracy and is expected to drift over time.

    OUTPUT AT THE SERIAL PORT
    Starting reed switch..
    ...........
    BTN Update [1] : MAGNET
    BTN State : 1
    BTN Update [2] : NO MAGNET
    BTN State : 0
    BTN Update [3] : MAGNET
    BTN State : 1
*/

#define REED_INPUT 2
#define SERIAL_SPEED 9600

#include <Arduino.h>
#include <Bounce2.h>

Bounce reedSwitch = Bounce();

uint32_t reedCounter = 0;

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
  Serial.begin(SERIAL_SPEED);

  while (!Serial)
      delay(1000); // remove this while loop to start the sketch at power-on, else the sketch will wait for a serial connection

  Serial.println(F(""));
  Serial.println(F("Starting reed switch.."));

  reedSwitch.attach(REED_INPUT, INPUT);
  reedSwitch.interval(25);
}

void loop()
{
  reedSwitch.update();

  if (reedSwitch.changed())
  {
    reedCounter = reedCounter + 1;

    Serial.print(F("BTN Update ["));
    Serial.print(reedCounter, DEC);
    Serial.print(F("] : "));

    Serial.println(reedSwitch.read() ? F("MAGNET") : F("NO MAGNET"));

    Serial.print(F("BTN State : "));
    Serial.println(reedSwitch.read(), DEC);
  }

  if(timerExpired())
    Serial.print(F("."));
}
