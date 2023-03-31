#include <VirtualWire.h>
int fsrAnalogPin = 0;
int fsrReading;
int val;
int value = 0;
void setup() 
{
  Serial.begin(115200);
  vw_set_tx_pin(12);
  vw_setup(2000);
}
void loop()
{
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  if(fsrReading==0 && value==0)
  {
    const char *data="a";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    value = 1;
  }
  else
  {
    const char *data="b";
    vw_send((uint8_t *)data, strlen(data));
    vw_wait_tx();
    value = 0;
  }
}
