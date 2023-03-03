/*
 This file is part of the AberSailbot minimum viable control system (AMVCS).
 AMVCS is free software: you can
 redistribute it and/or modify it under the terms of the GNU General Public
 License as published by the Free Software Foundation, version 2.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 51
 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 Copyright Colin Sauze
*/

#include <stdio.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <AsyncUDP.h>

#define HMC6343_ADDRESS 0x19
#define HMC6343_HEADING_REG 0x50

const char *ssid = "esp32";

AsyncUDP udp;

void setup() 
{
  Serial.begin(115200); //for GPS
  Serial.println("Setting up I2C...");
  Wire.begin(); // Initialise i2c for compass

  Serial.println("setting up WiFi");
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();

  Serial.println("Done");


}

//reads heading from HMC6343 compass
int readCompass() {
  byte buf[6];

  Wire.beginTransmission(HMC6343_ADDRESS); // Start communicating with the HMC6343 compasss
  Wire.write(HMC6343_HEADING_REG); // Send the address of the register that we want to read
  //Wire.write(0x55); // Send the address of the register that we want to read
  Wire.endTransmission();

  Wire.requestFrom(HMC6343_ADDRESS, 6); // Request six bytes of data from the HMC6343 compasss
  for(int i=0;i<6;i++)
  {
    while(Wire.available() < 1); // Busy wait while there is no byte to receive
    buf[i]=Wire.read();
    //printf("buf[%d]=%d\r\n",i,buf[i]);
  }
  int heading = ((buf[0] << 8) + buf[1]); // the heading in degrees
  int pitch =   ((buf[2] << 8) + buf[3]); // the pitch in degrees
  int roll = ((buf[4] << 8) + buf[5]); // the roll in degrees*/

  heading=heading/10;
  roll=roll/10;
  pitch=pitch/10;

  delay(100);

  return (int)heading; // Print the sensor readings to the serial port.
}

void loop()
{
    int heading = readCompass();
    String data = "Heading=" + String(heading) + "\n";
    Serial.print(data);
    //send data as UDP broadcast
    udp.broadcastTo(data.c_str(),1234);
}
