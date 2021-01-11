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
    Arduino : by Arduino LLC, subject to the GNU Lesser General Public License.  This library is in BETA and is available in the Arduino IDE and also at https://github.com/arduino/Arduino
    PMIC : by Arduino SA, subject to the GNU Lesser General Public License.  This library is available in the Arduino IDE and also at https://github.com/arduino-libraries/Arduino_BQ24195

    EXAMPLE NOTES
    1. Install the
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor and press the button, the serial connection speed is set by SERIAL_SPEED at 9600 bps

    A. Voltage calc is based on the 'MKRZERO_Read_Battery_Voltage by Arduino_Genuino' project on Arduino Create at https://create.arduino.cc
    B. The Arduino_PMIC is BETA code and is under development.
    C. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
    D. The timerExpired function has limited accuracy and is expected to drift over time.

    If you have problems compiling the code add the following #define to BQ24195.cpp  This is a known issue and is detailed at the link following. https://github.com/arduino-libraries/Arduino_BQ24195/issues/6

      #define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

    OUTPUT AT THE SERIAL PORT
    Starting PMIC test.. 1
    enableCharge() : 1
    getChargeCurrent() : 0.5120000243
    getChargeFault() : 0
    getChargeVoltage() : 4.1119999886
    getInputCurrentLimit() : 3.0000000000
    getInputVoltageLimit() : 3.8800001144
    getMinimumSystemVoltage() : 3.5000000000
    getPreChargeCurrent() : 0.2560000122
    getTermChargeCurrent() : 0.2560000122
    isBattConnected() : 0
    isBatteryInOverVoltage() : 0
    isHot() : 0
    isPowerGood() : 1
    isWatchdogExpired() : 0
    canRunOnBattery() : 0
    chargeStatus() : 0x20 FAST_CHARGING
    PMIC.chargeStatus() : 0x20 FAST_CHARGING
    ADC_BATTERY : 4.18
*/

#include <Arduino.h>
#include <Arduino_PMIC.h>

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
      delay(1000);

  Serial.println("");
  Serial.print(F("Starting PMIC test.. "));
  Serial.println(PMIC.begin(), DEC);

  Serial.print(F("enableCharge() : "));
  Serial.println(PMIC.enableCharge(), DEC);

  Serial.print(F("getChargeCurrent() : "));
  Serial.println(PMIC.getChargeCurrent(), DEC);

  Serial.print(F("getChargeFault() : "));
  Serial.println(PMIC.getChargeFault(), DEC);

  Serial.print(F("getChargeVoltage() : "));
  Serial.println(PMIC.getChargeVoltage(), DEC);

  Serial.print(F("getInputCurrentLimit() : "));
  Serial.println(PMIC.getInputCurrentLimit(), DEC);

  Serial.print(F("getInputVoltageLimit() : "));
  Serial.println(PMIC.getInputVoltageLimit(), DEC);

  Serial.print(F("getMinimumSystemVoltage() : "));
  Serial.println(PMIC.getMinimumSystemVoltage(), DEC);

  Serial.print(F("getPreChargeCurrent() : "));
  Serial.println(PMIC.getPreChargeCurrent(), DEC);

  Serial.print(F("getTermChargeCurrent() : "));
  Serial.println(PMIC.getTermChargeCurrent(), DEC);

  Serial.print(F("isBattConnected() : "));
  Serial.println(PMIC.isBattConnected(), DEC);

  Serial.print(F("isBatteryInOverVoltage() : "));
  Serial.println(PMIC.isBatteryInOverVoltage(), DEC);

  Serial.print(F("isHot() : "));
  Serial.println(PMIC.isHot(), DEC);

  Serial.print(F("isPowerGood() : "));
  Serial.println(PMIC.isPowerGood(), DEC);

  Serial.print(F("isWatchdogExpired() : "));
  Serial.println(PMIC.isWatchdogExpired(), DEC);

  Serial.print(F("canRunOnBattery() : "));
  Serial.println(PMIC.canRunOnBattery(), DEC);

  Serial.print(F("chargeStatus() : 0x"));
  Serial.print(PMIC.chargeStatus(), HEX);

  switch (PMIC.chargeStatus())
  {
  case NOT_CHARGING : Serial.println(F(" NOT_CHARGING")); break;
  case PRE_CHARGING : Serial.println(F(" PRE_CHARGING")); break;
  case FAST_CHARGING : Serial.println(F(" FAST_CHARGING")); break;
  case CHARGE_TERMINATION_DONE : Serial.println(F(" CHARGE_TERMINATION_DONE")); break;

  default: Serial.println(F(" Error!")); break;
  }
  //PMIC.end();
}

void loop()
{
  if (timerExpired(10e3)) // 10e3 = 10,000 = 10 seconds
  {
    Serial.print(F("PMIC.chargeStatus() : 0x"));
    Serial.print(PMIC.chargeStatus(), HEX);

    switch (PMIC.chargeStatus())
    {
      case NOT_CHARGING : Serial.println(F(" NOT_CHARGING")); break;
      case PRE_CHARGING : Serial.println(F(" PRE_CHARGING")); break;
      case FAST_CHARGING : Serial.println(F(" FAST_CHARGING")); break;
      case CHARGE_TERMINATION_DONE : Serial.println(F(" CHARGE_TERMINATION_DONE")); break;

      default: Serial.println(F(" Error!")); break;
    }

    Serial.print(F("ADC_BATTERY : "));
    Serial.println(analogRead(ADC_BATTERY) * (4.3 / 1023.0), DEC); // See note A for info on the conversion
  }
}
