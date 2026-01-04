#include <RH_ASK.h>
//#include <SPI.h>

#define SPEED_DEAD_ZONE 50

const int pinA = 3;
const int pinB = 11;

const int motorA = 6;
const int motorB = 5;

const int potIn = A0;

int real_angle;

int target_angle = 90;

int car_speed = 0;

RH_ASK driver(2000, 13, 12);

void setup() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);

  pinMode(potIn, INPUT);

  Serial.begin(9600);
  
  if (!driver.init())
    Serial.println("init failed");
}

void loop() {
  real_angle = map(analogRead(potIn),0,682,0,180);
  //Serial.print("Real Angle: ");
  //Serial.print(real_angle);
  //Serial.print(" Target Angle: ");
  //Serial.println(target_angle);

  byte inBuffer[2];
  uint8_t bufffer_len = sizeof(inBuffer);

  if (driver.recv(inBuffer, &bufffer_len))
  {
    //Serial.println("received");
    //driver.printBuffer("Got:", inBuffer, bufffer_len);
    
    target_angle = inBuffer[0];
  
    if (target_angle < 45)
      target_angle = 45;
    else if (target_angle > 120)
      target_angle = 120;
    
    car_speed = (inBuffer[1]+2-128)*2;

    if (abs(car_speed) > SPEED_DEAD_ZONE)
    {
      if (car_speed > 0)
      {
        car_speed -= SPEED_DEAD_ZONE;
        car_speed = map(car_speed,0,255-SPEED_DEAD_ZONE,50,255);
      }
      else if (car_speed < 0)
      {
        car_speed += SPEED_DEAD_ZONE;
        car_speed = map(car_speed,0,-(255-SPEED_DEAD_ZONE),-50,-255);
      }
    }
    else
    {
      car_speed = 0;
    }

    Serial.print("Target angle set to: ");
    Serial.print(target_angle);

    Serial.print("  Target speed set to: ");
    Serial.println(car_speed);
  }

  if (abs(target_angle-real_angle) < 3)
  {
    //Serial.println("Target Angle Reached");
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
  }
  else if (target_angle > real_angle)
  {
    analogWrite(pinA, 110+abs(target_angle-real_angle));
    analogWrite(pinB, 0);
    
  }
  else if (target_angle < real_angle)
  {
    analogWrite(pinA, 0);
    analogWrite(pinB, 110+abs(target_angle-real_angle));
    //Serial.println(150+abs(target_angle-real_angle));
    //Serial.println("Smaller");
  }

  if (car_speed > 0)
  {
    //Serial.println("Forward");
    analogWrite(motorA, car_speed);
    analogWrite(motorB, 0);
  }
  else if (car_speed < 0)
  {
    //Serial.println("Back");
    analogWrite(motorA, 0);
    analogWrite(motorB, abs(car_speed));
  }
  else
  {
    //Serial.println("Stop");
    analogWrite(motorA, 0);
    analogWrite(motorB, 0);
  }
  
}
