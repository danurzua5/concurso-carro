#include <AFMotor.h>

AF_DCMotor motorDE(2);
AF_DCMotor motorIZQ(3);

// Pines para los sensores de color (IZQ y DER)
const int s0=7, s1=8, s2=10, s3=11, out=12; //Sensor IZQ
const int ds0=1, ds1=0, ds2=2, ds3=3, dout=13; //Sensor DER

// Variables para almacenar los valores de calibración
int whiteValue, greenValue, orangeValue;
int threshold = 30; // Umbral para diferenciar colores

void setup(){
  Serial.begin(9600);
  
  // Configuración Sensor IZQ
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1,HIGH);
  
  // Configuración Sensor DER
  pinMode(ds0,OUTPUT);
  pinMode(ds1,OUTPUT);
  pinMode(ds2,OUTPUT);
  pinMode(ds3,OUTPUT);
  pinMode(dout,INPUT);
  digitalWrite(ds0, HIGH);
  digitalWrite(ds1,HIGH);
  
  // Calibración inicial
  calibrateSensors();
  delay(3000);
}

void loop(){
  // Leer valores actuales de los sensores
  int izqR = getRedIZQ();
  int izqG = getGreenIZQ();
  int izqB = getBlueIZQ();
  
  int derR = getRedDER();
  int derG = getGreenDER();
  int derB = getBlueDER();
  
  // Determinar qué color está detectando cada sensor
  bool izqOnGreen = isGreen(izqR, izqG, izqB);
  bool derOnGreen = isGreen(derR, derG, derB);
  bool izqOnOrange = isOrange(izqR, izqG, izqB);
  bool derOnOrange = isOrange(derR, derG, derB);
  
  // Lógica de movimiento
  if (izqOnOrange || derOnOrange) {
    // Si detecta naranja, detenerse o retroceder
    motorDE.run(RELEASE);
    motorIZQ.run(RELEASE);
    delay(500);
    motorDE.run(BACKWARD);
    motorIZQ.run(BACKWARD);
    delay(300);
  }
  else if (izqOnGreen && derOnGreen) {
    // Ambos sensores en verde - avanzar
    motorDE.setSpeed(255);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(255);
    motorIZQ.run(FORWARD);
  }
  else if (izqOnGreen && !derOnGreen) {
    // Solo izquierda en verde - girar izquierda
    motorDE.setSpeed(255);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(100);
    motorIZQ.run(FORWARD);
  }
  else if (!izqOnGreen && derOnGreen) {
    // Solo derecha en verde - girar derecha
    motorDE.setSpeed(100);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(255);
    motorIZQ.run(FORWARD);
  }
  else {
    // Ningún sensor en verde - buscar línea
    motorDE.setSpeed(150);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(150);
    motorIZQ.run(BACKWARD);
    delay(200);
  }
  
  delay(50); // Pequeña pausa para estabilidad
}

// Funciones para leer colores de cada sensor
int getRedIZQ(){
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  return pulseIn(out, LOW);
}

int getGreenIZQ(){
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  return pulseIn(out, LOW);
}

int getBlueIZQ(){
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  return pulseIn(out, LOW);
}

int getRedDER(){
  digitalWrite(ds2, LOW);
  digitalWrite(ds3, LOW);
  return pulseIn(dout, LOW);
}

int getGreenDER(){
  digitalWrite(ds2, HIGH);
  digitalWrite(ds3, HIGH);
  return pulseIn(dout, LOW);
}

int getBlueDER(){
  digitalWrite(ds2, LOW);
  digitalWrite(ds3, HIGH);
  return pulseIn(dout, LOW);
}

// Función de calibración
void calibrateSensors() {
  Serial.println("Calibrando sensores... Coloque el sensor IZQ sobre la línea verde");
  delay(3000);
  greenValue = getGreenIZQ();
  
  Serial.println("Coloque el sensor IZQ sobre la línea naranja");
  delay(3000);
  orangeValue = getGreenIZQ();
  
  Serial.println("Coloque el sensor IZQ sobre la base blanca");
  delay(3000);
  whiteValue = getGreenIZQ();
  
  Serial.println("Calibración completada");
  Serial.print("Verde: "); Serial.println(greenValue);
  Serial.print("Naranja: "); Serial.println(orangeValue);
  Serial.print("Blanco: "); Serial.println(whiteValue);
}

// Funciones para determinar colores
bool isGreen(int r, int g, int b) {
  // Comparar principalmente el valor verde
  return abs(g - greenValue) < threshold && abs(g - orangeValue) > threshold;
}

bool isOrange(int r, int g, int b) {
  // El naranja tiene componentes rojo y verde
  return abs(g - orangeValue) < threshold && abs(r - orangeValue) < threshold;
}
