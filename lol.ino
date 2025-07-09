#include <AFMotor.h>

AF_DCMotor motorDE(2);
AF_DCMotor motorIZQ(3);

// Pines para los sensores de color (IZQ y DER)
const int s0=7, s1=8, s2=10, s3=11, out=12; //Sensor IZQ
const int ds0=1, ds1=0, ds2=2, ds3=3, dout=13; //Sensor DER

// Variables para almacenar los valores de calibración
int greenR, greenG, greenB;
int orangeR, orangeG, orangeB;
int whiteR, whiteG, whiteB;
int threshold = 25; // Umbral ajustado para mejor precisión

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
  delay(2000);
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
  
  // Lógica de movimiento mejorada
  if (izqOnOrange || derOnOrange) {
    // Si detecta naranja, retroceder y buscar línea verde
    motorDE.run(RELEASE);
    motorIZQ.run(RELEASE);
    delay(200);
    motorDE.setSpeed(200);
    motorDE.run(BACKWARD);
    motorIZQ.setSpeed(200);
    motorIZQ.run(BACKWARD);
    delay(150);
  }
  else if (izqOnGreen && derOnGreen) {
    // Ambos sensores en verde - avanzar recto
    motorDE.setSpeed(255);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(255);
    motorIZQ.run(FORWARD);
  }
  else if (izqOnGreen && !derOnGreen) {
    // Solo izquierda en verde - girar izquierda suave
    motorDE.setSpeed(255);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(180);
    motorIZQ.run(FORWARD);
  }
  else if (!izqOnGreen && derOnGreen) {
    // Solo derecha en verde - girar derecha suave
    motorDE.setSpeed(180);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(255);
    motorIZQ.run(FORWARD);
  }
  else {
    // Ningún sensor en verde - buscar línea
    motorDE.setSpeed(120);
    motorDE.run(FORWARD);
    motorIZQ.setSpeed(120);
    motorIZQ.run(BACKWARD);
    delay(100);
  }
  
  delay(30); // Pausa reducida para mejor respuesta
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

// Función de calibración mejorada
void calibrateSensors() {
  Serial.println("=== CALIBRACIÓN DE SENSORES ===");
  
  Serial.println("Coloque el sensor IZQ sobre la línea VERDE");
  delay(3000);
  greenR = getRedIZQ();
  greenG = getGreenIZQ();
  greenB = getBlueIZQ();
  
  Serial.println("Coloque el sensor IZQ sobre la línea NARANJA");
  delay(3000);
  orangeR = getRedIZQ();
  orangeG = getGreenIZQ();
  orangeB = getBlueIZQ();
  
  Serial.println("Coloque el sensor IZQ sobre la base BLANCA");
  delay(3000);
  whiteR = getRedIZQ();
  whiteG = getGreenIZQ();
  whiteB = getBlueIZQ();
  
  Serial.println("=== VALORES DE CALIBRACIÓN ===");
  Serial.print("Verde - R:"); Serial.print(greenR); Serial.print(" G:"); Serial.print(greenG); Serial.print(" B:"); Serial.println(greenB);
  Serial.print("Naranja - R:"); Serial.print(orangeR); Serial.print(" G:"); Serial.print(orangeG); Serial.print(" B:"); Serial.println(orangeB);
  Serial.print("Blanco - R:"); Serial.print(whiteR); Serial.print(" G:"); Serial.print(whiteG); Serial.print(" B:"); Serial.println(whiteB);
  Serial.println("Calibración completada!");
}

// Funciones mejoradas para determinar colores
bool isGreen(int r, int g, int b) {
  // Verificar que el verde sea dominante y esté cerca del valor calibrado
  int greenDiff = abs(g - greenG);
  int orangeDiff = abs(g - orangeG);
  int whiteDiff = abs(g - whiteG);
  
  return greenDiff < threshold && greenDiff < orangeDiff && greenDiff < whiteDiff;
}

bool isOrange(int r, int g, int b) {
  // El naranja tiene componentes rojo y verde similares
  int orangeRDiff = abs(r - orangeR);
  int orangeGDiff = abs(g - orangeG);
  int greenDiff = abs(g - greenG);
  
  return orangeRDiff < threshold && orangeGDiff < threshold && orangeGDiff < greenDiff;
}