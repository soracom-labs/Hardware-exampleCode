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
    ArduinoJson : by Benoti Blanchon, subject to the MIT License.  This library is available in the Arduino IDE and also at https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

    EXAMPLE NOTES
    1. Install an active SIM into your MKR GSM 1400 module
    2. Make the USB connection between the Arduino MKR 1400 your Arduino IDE (PC)
    3. Use the Arduino IDE to verify and upload the Arduino image to the MKR GPS 1400, you may need to install additional libraries from the IDE (CTRL SHIFT I) to verify the sketch.
    4. Start the serial monitor, the serial connection speed is set by SERIAL_SPEED at 9600 bps
    
    OUTPUT AT THE SERIAL PORT
    16:58:54.592 -> 
    16:58:54.592 -> Starting Soracom CONFIG test.. 
    16:58:54.592 -> Starting Modem, GSM, GPRS and Soracom Cloud connection 
    16:58:54.742 -> 	[1/5] GSM_READY * Modem IMEI : 357520078534019 SIM ICCID : 8942310019001628528
    16:59:05.393 -> 	[2/5] GPRS_READY *
    16:59:09.008 -> 	[3/5] Cloud Connection TRUE *
    16:59:09.738 -> .	[4/5] Requesting update from Soracom Metadata
    16:59:10.733 -> 
    16:59:10.733 -> GET /v1/userdata HTTP/1.1
    16:59:10.733 -> Host: metadata.soracom.io
    16:59:10.766 -> User-Agent: Soracom/1.0
    16:59:10.799 -> Accept: application/json
    16:59:10.832 -> X-Clacks-Overhead: GNU Terry Pratchett
    16:59:10.832 -> Connection: close
    16:59:10.866 -> 
    16:59:10.899 -> .	[5/5] Awaiting reply
    16:59:11.960 -> 
    16:59:11.960 -> HTTP/1.1 200 OK
    16:59:11.960 -> Content-Type: application/json
    16:59:11.993 -> Content-Length: 53
    16:59:12.026 -> Date: Thu, 31 Dec 2020 16:59:11 GMT
    16:59:12.059 -> Connection: close
    16:59:12.092 -> 
    16:59:12.092 -> {"PTCL":"HTTP","HST":"harvest.soracom.io","PRT":"80"}
    16:59:12.225 -> 
    16:59:12.225 -> Configuration from Soracom Metadata service
    16:59:12.225 -> Application Protocol :	HTTP
    16:59:12.258 -> Interface Name :	harvest.soracom.io
    16:59:12.258 -> Interface Port :	80
    16:59:12.258 -> 
    16:59:12.258 -> client.stop
    16:59:12.258 -> gprs.detachGPRS : 1
    16:59:12.955 -> gsmAccess.shutdown : 1
*/

#include <Arduino.h>
#include <MKRGSM.h>
#include <ArduinoJson.h>

const char http_request_0[] PROGMEM = "GET /v1/userdata HTTP/1.1\r\n";
const char http_request_1[] PROGMEM = "Host: metadata.soracom.io\r\n";
const char http_request_2[] PROGMEM = "User-Agent: Soracom/1.0\r\n";
const char http_request_3[] PROGMEM = "Accept: application/json\r\n";
const char http_request_4[] PROGMEM = "X-Clacks-Overhead: GNU Terry Pratchett\r\n";
const char http_request_5[] PROGMEM = "Connection: close\r\n";
const char http_request_6[] PROGMEM = "\r\n";
const char http_request_7[] PROGMEM = ""; // must end with an empty string

const char *const http_request[] PROGMEM = {http_request_0, http_request_1, http_request_2, http_request_3, http_request_4, http_request_5, http_request_6, http_request_7};

#define PINNUMBER ""
#define GPRS_APN "soracom.io"
#define GPRS_LOGIN "sora"
#define GPRS_PASSWORD "sora"

#define METADATA_SERVER "metadata.soracom.io"
#define METADATA_PORT 80

GSMModem modem;
GSM gsmAccess(false); // set true to debug modem AT commands
GPRS gprs;
GSMClient client(true); // calls are Sync

boolean notConnected = true;

void setup()
{
  Serial.begin(9600);
  SerialGSM.begin(115200);

  delay(1000);

  while (!Serial)
    delay(1000);

  Serial.println();
  Serial.println(F("Starting Soracom CONFIG test.. "));

  while (!SerialGSM)
    delay(1000);

  Serial.println(F("Starting Modem, GSM, GPRS and Soracom Cloud connection "));

  pinMode(LED_BUILTIN, OUTPUT);
  arduino::digitalWrite(LED_BUILTIN, HIGH);

  delay(250);
  arduino::digitalWrite(LED_BUILTIN, LOW);

  gsmAccess.setTimeout(15e3);
  gprs.setTimeout(15e3);
  client.setTimeout(15e3);

  while (notConnected)
  {
    boolean gprs_status = false;

    arduino::digitalWrite(LED_BUILTIN, LOW);

    while((gsmAccess.isAccessAlive() != 1))
    {
      gprs_status = false;
      client.stop();

      Serial.print(F("\t[1/5] "));
      switch (gsmAccess.begin(PINNUMBER, true, true))
      {
      case ERROR:                 Serial.println(F("ERROR")); break;
      case IDLE:                  Serial.println(F("IDLE")); break;
      case CONNECTING:            Serial.println(F("CONNECTING")); break;
      case GPRS_READY:            Serial.println(F("GPRS_READY")); break;
      case TRANSPARENT_CONNECTED: Serial.println(F("TRANSPARENT_CONNECTED")); break;
      case GSM_OFF:               Serial.println(F("GSM_OFF")); break;
      case GSM_READY:             Serial.print(F("GSM_READY *")); Serial.print(F(" Modem IMEI : ")); Serial.print(modem.getIMEI()); Serial.print(F(" SIM ICCID : ")); Serial.println(modem.getICCID()); break;

      default:                    Serial.println(F("default - err?")); break;
      }
    }

    while(gprs_status == false)
    {
      client.stop();
      gprs.detachGPRS();

      Serial.print(F("\t[2/5] "));
      switch (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD, true))
      {
      case ERROR:                 Serial.println(F("ERROR")); break;
      case IDLE:                  Serial.println(F("IDLE")); break;
      case CONNECTING:            Serial.println(F("CONNECTING *")); gprs_status = true; break;
      case GSM_READY:             Serial.println(F("GSM_READY *")); gprs_status = true; break;
      case GPRS_READY:            Serial.println(F("GPRS_READY *")); gprs_status = true; break;
      case TRANSPARENT_CONNECTED: Serial.println(F("TRANSPARENT_CONNECTED")); break;

      default:                    Serial.println(F("default - err?")); break;
      }
    }

    while(client != true)
    {
      client.stop();

      Serial.print(F("\t[3/5] Cloud Connection "));
      switch (client.connect(METADATA_SERVER, METADATA_PORT))
      {
      case true :                 Serial.println(F("TRUE *")); notConnected = false; break;
      case false :                Serial.println(F("FALSE")); break;

      default: break;             Serial.println(F("default - err?")); break;
      }
    }

    Serial.print(F("."));
    delay(1000);
  }

  if (notConnected == false)
  {
    Serial.println(F("\t[4/5] Requesting update from Soracom Metadata"));

    arduino::digitalWrite(LED_BUILTIN, HIGH);


    Serial.println();

    for (int i = 0; strcmp(http_request[i], "") != 0; i++)
    {
      Serial.print(http_request[i]);

      client.print(http_request[i]);
    }

    arduino::digitalWrite(LED_BUILTIN, LOW);
  }

  DynamicJsonDocument soracomMetadataConfig(100);

  while (!client.available())
  {
    Serial.print(F("."));
    delay(1000);
  }

  Serial.println(F("\t[5/5] Awaiting reply"));
  Serial.println();

  String buffer;
  buffer.reserve(100);

  while (client.available())
  {
    char byte = static_cast<char>(client.read());
    Serial.print(byte);
    buffer = buffer + byte;
  }

  Serial.println();

  deserializeJson(soracomMetadataConfig, buffer.substring(buffer.indexOf("{")));

  const char *ptcl = soracomMetadataConfig[F("PTCL")];
  const char *host = soracomMetadataConfig[F("HST")];
  const char *prt = soracomMetadataConfig[F("PRT")];

  Serial.println();
  Serial.println(F("Configuration from Soracom Metadata service"));
  Serial.print(F("Application Protocol :\t"));
  Serial.println(ptcl);

  Serial.print(F("Interface Name :\t"));
  Serial.println(host);

  Serial.print(F("Interface Port :\t"));
  Serial.println(prt);

  Serial.println();
  Serial.println(F("client.stop"));
  client.stop();

  Serial.print(F("gprs.detachGPRS : "));
  Serial.println(gprs.detachGPRS());

  Serial.print(F("gsmAccess.shutdown : "));
  Serial.println(gsmAccess.shutdown());
}
void loop()
{
  delay(1000);
}
