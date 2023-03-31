#include <SoftwareSerial.h>
#include <Wire.h>
#include <TinyGPS.h>
#include <RH_ASK.h>
#include <SPI.h>
RH_ASK rf_driver;
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
float flat, flon;
unsigned long age;
int angleChange=0;
int fsrReading_2;
int fsrPin = 0;
int value = 0;
bool newData = false;
unsigned long chars;
unsigned short sentences, failed;
SoftwareSerial mySerial(3, 2);
SoftwareSerial GPSserial(12,11);
TinyGPS gps;
void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  rf_driver.init();
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(500);
}
void loop()
{
 mpu_read();
 ax = (AcX-2050)/16384.00;
 ay = (AcY-77)/16384.00;
 az = (AcZ-1947)/16384.00;
 gx = (GyX+270)/131.07;
 gy = (GyY-351)/131.07;
 gz = (GyZ+136)/131.07;
 float Raw_AM = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
 int angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
 int AM = Raw_AM * 10;
 if((angleChange>10)&&(AM!=7&AM!=8))
 {
  Serial.println("FALL DETECTED");
  seat_read();
  delay(500);
  }
 else
 {
  Serial.println("NO FALL DETECTED");}
  delay(1000);
  }
void mpu_read()
{
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);
 AcX=Wire.read()<<8|Wire.read();
 AcY=Wire.read()<<8|Wire.read();
 AcZ=Wire.read()<<8|Wire.read();
 Tmp=Wire.read()<<8|Wire.read();
 GyX=Wire.read()<<8|Wire.read();
 GyY=Wire.read()<<8|Wire.read();
 GyZ=Wire.read()<<8|Wire.read();
}
void gsm_read()
{
  gps_read();
  Serial.println("Initializing...");
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1");
  updateSerial();
  mySerial.println("AT+CMGS=\"+917604912953\"");
  updateSerial();
  mySerial.println("Accident Detected");
  updateSerial();
  mySerial.print("http://maps.google.com/maps?q=loc:");
  updateSerial();
  mySerial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
  updateSerial();
  mySerial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
  updateSerial();
  mySerial.println();
  updateSerial();
  mySerial.write(26);
}
void gps_read()
{
    gps.f_get_position(&flat, &flon);
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());
  }
  while(mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}
void seat_read()
{
  fsrReading_2 = analogRead(fsrPin);
  if(fsrReading_2>1000 && value==0)
  {
    Serial.println("Person in the Seat");
    helmet_tilt_read();
  }
  else
  {
    Serial.println("Person not in the Seat");
    gsm_read();
  }
}
void helmet_tilt_read()
{
   uint8_t buf[11];
   uint8_t buflen = sizeof(buf);
   if (rf_driver.recv(buf, &buflen))
    {
      if((char*)buf=="a")
      {
       Serial.println("Person is Conscious");
       loop();
      }
      else if((char*)buf=="b")
      {
        Serial.println("Person is Unconscious");
        gsm_read();
      }
    }
}
