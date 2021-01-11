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
    MKRGSM : by Arduino AG, subject to the GNU Lesser General Public License.  This library is available in the Arduino IDE and also at https://github.com/arduino-libraries/MKRGSM

    EXAMPLE NOTES
    1. Install your Soracom SIM card into the MKR GSM 1400
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC).  Your USB PSU (Power Supply) should be rated for at least 2A, the radio transmitter when operating requires high current.
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor, the serial connection speed is set by SERIAL_SPEED at 9600 bps

    A. A modem reset can take up to 10 seconds, gsmAccess.begin() will force a hard reset on the modem
    B. The 'F("STRING")' construct places static data into flash storage, preserving RAM for your code.
    C. Refer to the Soracom Console (HARVEST) where you should be able see the data from this example loaded into the dashboard
    D. LED_BUILTIN will light for the time it takes to send the message to the network interface for Soracom HARVEST.  LED_BUILTIN is the LED close to the I2C interface and on the same side of the MKR module as the GREEN power LED.

    OUTPUT AT THE SERIAL PORT
    Starting Soracom HARVEST test.. Starting Modem, GSM, GPRS and Soracom Cloud connection
            [1/4] GSM_READY  Modem IMEI : 357520078534019 SIM ICCID : 8942310019001628528
            [2/4] GPRS_READY
            [3/4] Soracom Cloud connection ready
    .       [4/4] Sending message to Soracom Harvest
    Soracom Cloud connection is UP, sending '{"A":10,"B":11,"C":12}', timeout is : 1000
    HTTP/1.1 201 Created
    Date: Tue, 29 Dec 2020 16:50:10 GMT
    Connection: close
    Content-Length: 0
*/

#include <Arduino.h>
#include <MKRGSM.h>

#define PINNUMBER ""
#define GPRS_APN "soracom.io"
#define GPRS_LOGIN "sora"
#define GPRS_PASSWORD "sora"

#define HARVEST_SERVER "unified.soracom.io"
#define HARVEST_PORT 80

GSMModem modem;
GSM gsmAccess(false); // set true to debug modem AT commands
GPRS gprs;
GSMClient client;

boolean notConnected = true;

void setup()
{
  Serial.begin(9600);
  SerialGSM.begin(115200);

  delay(1000);

  for(int loopCount = 0; loopCount <= 60; loopCount++)
      while (!Serial)
          delay(1000); // Wait for a serial connection, timeout after 60 loops (close to 1 min)

  Serial.println(F(""));
  Serial.print(F("Starting Soracom HARVEST test.. "));

  while (!SerialGSM)
    delay(1000);

  Serial.println(F("Starting Modem, GSM, GPRS and Soracom Cloud connection"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  delay(250);
  digitalWrite(LED_BUILTIN, LOW);

  while (notConnected)
  {
    digitalWrite(LED_BUILTIN, LOW);

    if (gsmAccess.begin(PINNUMBER, true, true) == GSM_READY)
    {
      delay(1000);
      Serial.print(F("\t[1/4] GSM_READY "));
      Serial.print(F(" Modem IMEI : "));
      Serial.print(modem.getIMEI());
      Serial.print(F(" SIM ICCID : "));
      Serial.println(modem.getICCID());

      if (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD, true) == GPRS_READY)
      {
        delay(1000);
        Serial.println(F("\t[2/4] GPRS_READY "));

        if (client.connect(HARVEST_SERVER, HARVEST_PORT) == true)
        {
          delay(1000);
          Serial.println(F("\t[3/4] Soracom Cloud connection ready "));
          notConnected = false;
        }
        else
        {
          client.stop();
        }
      }
      else
      {
        gprs.detachGPRS();
      }
    }
    else
    {
      gsmAccess.shutdown();
    }

    Serial.print(F("."));
    delay(1000);
  }

  if (client.connect(HARVEST_SERVER, HARVEST_PORT))
  {
    Serial.println(F("\t[4/4] Sending message to Soracom Harvest"));

    digitalWrite(LED_BUILTIN, HIGH);

    char message[] = "{\"A\":10,\"B\":11,\"C\":12}";

    Serial.print(F("Soracom Cloud connection is UP, sending '"));
    Serial.print(message);
    Serial.print(F("', timeout is : "));
    Serial.println(client.getTimeout(), DEC);

    client.println(F("POST / HTTP/1.1"));
    client.print(F("Host: "));
    client.println(HARVEST_SERVER);
    client.println(F("User-Agent: Soracom/1.0"));
    client.println(F("Connection: close"));
    client.println(F("Content-type: application/json"));
    client.println(F("X-Clacks-Overhead: GNU Terry Pratchett"));

    client.print(F("Content-Length: "));
    client.println(strlen(message));

    client.println();
    client.println(message);

    if (client.ready() == 0)
    {
      Serial.print(F("."));
      delay(500);
    }

    digitalWrite(LED_BUILTIN, LOW);
  }

  while (client.connected() || client.available())
  {
    if (client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
  }

  client.stop();
  gprs.detachGPRS();
  gsmAccess.shutdown();
}
void loop()
{
}
