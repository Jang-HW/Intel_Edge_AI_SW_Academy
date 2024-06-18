#include<Servo.h> 
Servo servo;  
int value = 0;  

void setup() {
  servo.attach(7); 
  Serial.begin(9600);
}

void loop() {
  if(Serial.available())    
  {
    char in_data;           
    in_data = Serial.read(); 
    if(in_data == '1')      
    {
      value += 30;          
      if(value == 210)       
        value = 0;          
    }
      
    servo.write(value); 
    
    Serial.println(value);    
  }
}