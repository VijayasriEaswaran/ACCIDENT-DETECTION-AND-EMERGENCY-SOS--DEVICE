#include <VirtualWire.h>
int in1=2;
int in2=4;
int enA=3;
void setup()
{
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    vw_set_rx_pin(11);
    vw_setup(2000);
    vw_rx_start();
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN; 
 
    if (vw_get_message(buf, &buflen))
    {
      if(buf[0]=='a')
      {
       Serial.println("print");
       digitalWrite(in1,HIGH);
       digitalWrite(in2,LOW);
       digitalWrite(enA, 100);
      }
      else if(buf[0]=='b')
      {
       delay(2000);
      }
    }
}
