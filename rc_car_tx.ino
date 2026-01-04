#include <RH_ASK.h>
#include <SPI.h>

const int steering_in = A0;
const int speed_in = A1;

int car_speed = 128;
int steering_angle = 90;

RH_ASK driver(2000, 12, 13);

void setup() {
  pinMode(steering_in, INPUT);
  pinMode(speed_in, INPUT);

  Serial.begin(9600);

  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  steering_angle = map(analogRead(steering_in),0,1024,45,120);
  car_speed = map(analogRead(speed_in),0,1024,0,255);
  //car_speed = 128;

  byte outBuffer[2] = {(byte)steering_angle, (byte)car_speed};

  driver.send((byte *)outBuffer, 2);

  driver.waitPacketSent();

  delay(10);
}
