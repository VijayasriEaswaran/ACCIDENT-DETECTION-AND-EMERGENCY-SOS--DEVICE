
#include <VirtualWire.h>
#define Tilt 6
#define buzz 7
const int datain = 10;
char *data;
bool ce;
void setup()
{
Serial.begin(9600);
pinMode(Tilt, INPUT);
pinMode(buzz, OUTPUT);
vw_set_ptt_inverted(true);
vw_set_tx_pin(12);
vw_setup(4000);
}

void loop()
{
ce=digitalRead(Tilt);
if(ce==HIGH)
{
  digitalWrite(buzz,HIGH);
  delay(5000);
  digitalWrite(buzz,LOW);
  Serial.println("high");
  transmitter_1();
}
else
{
  digitalWrite(buzz,LOW);
  delay(1000);
  Serial.println("low");
  transmitter_2();
}
}
void transmitter_1()
{
  data="1";
  vw_send((uint8_t *)data, strlen(data));
  vw_wait_tx();
}
void transmitter_2()
{
  data="2";
  vw_send((uint8_t *)data, strlen(data));
  vw_wait_tx();
}
