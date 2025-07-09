#include <AFMotor.h>

//MOTORES
AF_DCMotor motorDE(3);
AF_DCMotor motorIZQ(2);
//SENSOR DERECHO
const int s0 = 1;  
const int s1 = 0;  
const int s2 = 2;  
const int s3 = 3;  
const int out = 13;    
int rojo = 0;  
int verde = 0;  
int azul = 0;  
//SENSOR IZQUIERDO
const int is0=7, is1=8, is2=10, is3=11, iout=12;
int irojo = 0;  
int iverde = 0;  
int iazul = 0;

void setup(){  
  Serial.begin(9600); 
  pinMode(s0,OUTPUT);  
  pinMode(s1,OUTPUT);  
  pinMode(s2,OUTPUT);  
  pinMode(s3,OUTPUT);  
  pinMode(out,INPUT);   
  digitalWrite(s0,HIGH);  
  digitalWrite(s1,HIGH);  

  pinMode(is0,OUTPUT);  
  pinMode(is1,OUTPUT);  
  pinMode(is2,OUTPUT);  
  pinMode(is3,OUTPUT);  
  pinMode(iout,INPUT);   
  digitalWrite(is0,HIGH);  
  digitalWrite(is1,HIGH); 
}  

   
void loop(){
  //SENSOR DERECHO  
  color();
    Serial.print("   ");
  Serial.println("  DERECHO");
    Serial.print("   ");  
  Serial.print(rojo, DEC);  
  Serial.print("   ");  
  Serial.print(verde, DEC);  
  Serial.print("   ");  
  Serial.print(azul, DEC);  
  
  //DERECHO
  if (rojo < 15 && (verde < 15) && rojo < 15) //BLANCO AVANZA
  {  
   Serial.println("   BLANCO");    
   motorDE.setSpeed(255);
   motorDE.run(FORWARD);
   motorIZQ.setSpeed(255);
   motorIZQ.run(FORWARD);
  }   
  else if (rojo < 15 && rojo < verde && verde > azul)  //ROJO A LA IZQUIERDA
  {  
   Serial.println("   Rojo");        
  }  
  else if (rojo > verde && azul > verde )  //VERDE A LA IZQUIERDA
  {  
   Serial.println("   Verde");       
  }  
  else{
  Serial.println("  ");  
  }
  
  delay(900);     
  }  
//SENSOR DERECHO 
  void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);   
  rojo = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);   
  azul = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);    
  verde = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
}
//SENSOR IZQUIERDO
  void icolor(){
  digitalWrite(is2, LOW);  
  digitalWrite(is3, LOW);   
  rojo = pulseIn(iout, digitalRead(iout) == HIGH ? LOW : HIGH);  
  digitalWrite(is3, HIGH);   
  azul = pulseIn(iout, digitalRead(iout) == HIGH ? LOW : HIGH);  
  digitalWrite(is2, HIGH);    
  verde = pulseIn(iout, digitalRead(iout) == HIGH ? LOW : HIGH);
}