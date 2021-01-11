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
*/

/*
 * To run hlwWorld demo sketch you will need to
 *    Download & install the the Arduino IDE
 *    Connect & Power the MKR GSM 1400 module
 *    Load & run the hloWorld sketch
 *
 * Plugin/Connect the MKR GSM 1400 device using a USB HUB or a USB port on your PC.  Check the power rating on the USB Port, programming the GSM modem and radio in the MKR GSM 1400 can draw a lot of power from the port.
 *
 * Download the Arduino IDE.                The Arduino IDE is freely available for Linux, MAC & Windows from
 *                                          https://www.arduino.cc/en/Guide, pick the correct install for your operating system, download and install.
 *                                          Once installed open the IDE and if prompted update to the current version.
 *
 * IDE : Tools -> Board -> Boards Manager   You will need to add the MKR GSM 1400 board into your development environment.
 *                                          In the search field on the library manager add 'MKR GSM 1400' and install 'Arduino SAMD Boards (32-bits ARM Cortex-Mo+)'.
 *
 * IDE : Tools -> Manage Library            You will need to add the MKR GSM 1400 library into your development environment.
 *                                          In the search field on the library manager add 'MKRGSM' and install the library of the same name.
 *
 *                                          Add the library to your build configuration, select 'MKR GSM 1400'.
 * IDE : Tools -> Board -> Arduino SAMD (32 bit ARM Cortex M0+) -> Arduino MKR GSM 1400
 *
 * IDE : Tools -> Port -> '/dev/ttyACM0 (Arduino MKR GSM 1400)'
 *                                          To enable serial communication with the board select the correct USB Serial Port from tools.
 *                                          On a Linux platform the prompt includes '/dev/ttyACM0', this may look different for other platforms
 *
 * IDE : File -> New                        Create a new sketch
 * IDE : File -> Save as                    Save the new sketch to a file, Overwrite the default sketch name with 'demo_hloWorld'
 *
 *
 * IDE : Sketch -> Include Library          The MKR GSM 1400 functions exposed through an Arduino library. Include the MKRGSM library in the sketch.
 *                                          Search in the list for MKRGSM and select.
 *                                          When you return to the IDE and your code you should see that an include statement has been added to the code ('#include <MKRGSM.h>')
 *
 * STEP 1 : hloWorld; the hloWorld sketch will write text to the serial port
 *                                          In the SETUP section of the code add the Serial.begin and the remaining code to your sketch.
 *                                          The value '9600' in Serial.begin is the port speed, we will set this in the IDE when we connect to the serial port on the Arduino MKG GSM 1400
 *
 * IDE : Sketch -> Verify/Compile           Check the code for errors and compile
 * IDE : Sketch -> Upload                   Upload the code into flash memory on the MKR GSM 1400, the device will reboot and you sketch will run when the upload is finished
 *
 * IDE : Tools -> Serial Monitor            Connect to the serial port on MKR GSM 1400 and observe serial output from the sketch
 *                                          On the new window check that the connection speed is a match for the connection speed in your sketch.
 *                                          Bottom Right Hand side second pulldown should be set to 9600.
 *
 * STEP 2 : blink; update the hloWorld sketch to flash one of the LEDs on the MKR GSM 1400
 *                                          Add the remaining code to the setup section of your sketch and also to
 *                                          loop section of the sketch.
 *
 * NB : The sketch uses the LOGICAL NOT construct in the code below ('!').  A Logical Not inverts a logic value, true becomes false and false becomes true
 *
 *      (!Serial)       Serial will return a Logical TRUE if a Serial connection has been established, in our code we need to wait for that connection.
 *      !digitalRead()  digitalRead returns the Logical state of the digital pin defined in the first argument, in our code we invert (Logical Not) the value returned and write it back to the same digital pin.  ON becomes OFF, OFF becomes ON.
 *
 * NB : Have a care when connecting hardware to Pins on your MKR GSM 1400.  Review the power requirement for the new device against the ratings for the pin(s) and the power budget for the whole setup.  Best case, you sketch will not work, worst case you will damage one of both of the devices.
 *      Spec's and drawings and PIN maps for the MKR GSM 1400 are online at the Arduino site.
 *      PIN's on the MKR GSM 1400 are limited to 3.3 volts and maximum current of 7 mA.  If you exceed these values you are likely to damage the board.
 *
 * NB : Antenna, your MKR GSM 1400 has been provided with an antenna, this should be connected to the board at the first opportunity, the antenna should be in place when the GSM radio is activated.
 *
 * NB : USB Power, installing a fully charged LiPo battery into the BATT port on the MKR GSM 1400 can help to mitigate demand on the USB Power source.
 *      A good reference for the above is online at https://www.tme.eu/Document/00e2e442e11233ad78ec4808c9124d43/ABX00018_MKR%20GSM%201400.pdf
 */

#include <Arduino.h>
#include <MKRGSM.h>

void setup() {

// STEP 1 : hloWorld
  Serial.begin(9600);                 // connection speed for the serial port, this should match the setting in the IDE.

  while (!Serial)                     // wait for the serial port to connect, this will wait forever
    delay(1000);

  Serial.println();
  Serial.println(F("Hello World!"));  // print the text 'Hello World!' to the serial output port

// STEP 2 : blink LED_BUILTIN

  pinMode(LED_BUILTIN, OUTPUT);       // set the hardware pin with the LED attached to OUTPUT, this sets the pin up as a driver (current source)
  digitalWrite(LED_BUILTIN, LOW);     // set the hardware pin to LOW, the LED is OFF
}

void loop() {
  static unsigned long timeStamp = 0;

  if(millis() - timeStamp > 1000)
  {
    timeStamp = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
                                      // change the state of the LED from OFF to ON / ON to OFF
    Serial.print(F("."));
                                      // at each one second interval the LED will switch and a '.' is  printed to the serial port
  }
}
