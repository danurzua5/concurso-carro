#include <AFMotor.h>  // Librería para control de motores

#define s0Der A4        // Sensor Derecho
#define s1Der A5
#define s2Der 6
#define s3Der 9
#define outDer 13

#define s0Izq A0         // Sensor Izquierdo
#define s1Izq A1
#define s2Izq A2
#define s3Izq A3
#define outIzq 2

// Motores
AF_DCMotor motor1(1);  // Motor izquierdo (M1)
AF_DCMotor motor2(2);  // Motor derecho (M2)

// Variables RGB
int Red1 = 0, Green1 = 0, Blue1 = 0; // Sensor derecho
int Red2 = 0, Green2 = 0, Blue2 = 0; // Sensor izquierdo

String colorObjetivo = "";
bool colorDetectado = false;

void setup() {
  Serial.begin(9600);

  // Configurar sensores derecho
  pinMode(s0Der, OUTPUT);
  pinMode(s1Der, OUTPUT);
  pinMode(s2Der, OUTPUT);
  pinMode(s3Der, OUTPUT);
  pinMode(outDer, INPUT);

  // Configurar sensores izquierdo
  pinMode(s0Izq, OUTPUT);
  pinMode(s1Izq, OUTPUT);
  pinMode(s2Izq, OUTPUT);
  pinMode(s3Izq, OUTPUT);
  pinMode(outIzq, INPUT);

  // Configurar frecuencia (modo recomendado)
  digitalWrite(s0Der, HIGH);
  digitalWrite(s1Der, HIGH);
  digitalWrite(s0Izq, HIGH);
  digitalWrite(s1Izq, HIGH);

  // Motores detenidos
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  StartMotor1();
  StartMotor2();

  delay(3000);
  Serial.println("Buscando color...");

  // Buscar color inicial (desde cualquiera de los sensores)
  while (!colorDetectado) {
    GetColorsDer();
    GetColorsIzq();
    StartMotor1();
    StartMotor2();

    String col1 = DetectarColor(Red1, Green1, Blue1);
    String col2 = DetectarColor(Red2, Green2, Blue2);

    if (col1 == "Verde" || col1 == "Naranja") {
      colorObjetivo = col1;
      colorDetectado = true;
    } else if (col2 == "Verde" || col2 == "Naranja") {
      colorObjetivo = col2;
      colorDetectado = true;
    } else {
      Serial.println("Buscando color...");
      delay(300);
    }
  }

  Serial.print("Color detectado: ");
  Serial.println(colorObjetivo);
}

void loop() {

  delay(500);
  GetColorsDer();
  GetColorsIzq();

  String colDer = DetectarColor(Red1, Green1, Blue1);
  String colIzq = DetectarColor(Red2, Green2, Blue2);

  // Ambos sensores en color objetivo (cruce o recta perfecta): avanzar recto
  if ((colDer == colorObjetivo) && (colIzq == colorObjetivo)) {
    StartMotor1();
    StartMotor2();
  }

  // Ambos sensores blanco: avanzar recto
  else if (colDer == "Blanco" && colIzq == "Blanco") {
    StartMotor1();
    StartMotor2();
  }

  // Ambos detectan el color alterno (cruce del otro color)
  else if (((colDer == "Verde" && colIzq == "Verde") && colorObjetivo == "Naranja") || 
           ((colDer == "Naranja" && colIzq == "Naranja") && colorObjetivo == "Verde")) {
    StartMotor1();
    StartMotor2();
  }

  // Sensor izquierdo detecta color objetivo, derecho blanco → girar derecha
  else if (colIzq == colorObjetivo && colDer == "Blanco") {
    StartMotor2();
    StopMotor1();
  }

  // Sensor derecho detecta color objetivo, izquierdo blanco → girar izquierda
  else if (colDer == colorObjetivo && colIzq == "Blanco") {
    StartMotor1();
    StopMotor2();
  }

  // Ninguno detecta el color objetivo → detenerse
  else {
    StopMotors();
  }

  // Mostrar en consola lo que ve cada sensor
  Serial.print("Izq: "); Serial.print(colIzq);
  Serial.print(" | Der: "); Serial.println(colDer);

  delay(50);
}

// ============================
// FUNCIONES DE COLOR
// ============================
void GetColorsDer() {
  digitalWrite(s2Der, LOW); digitalWrite(s3Der, LOW);
  Red1 = pulseIn(outDer, digitalRead(outDer) == HIGH ? LOW : HIGH);

  digitalWrite(s3Der, HIGH);
  Blue1 = pulseIn(outDer, digitalRead(outDer) == HIGH ? LOW : HIGH);

  digitalWrite(s2Der, HIGH);
  Green1 = pulseIn(outDer, digitalRead(outDer) == HIGH ? LOW : HIGH);
}

void GetColorsIzq() {
  digitalWrite(s2Izq, LOW); digitalWrite(s3Izq, LOW);
  Red2 = pulseIn(outIzq, digitalRead(outIzq) == HIGH ? LOW : HIGH);

  digitalWrite(s3Izq, HIGH);
  Blue2 = pulseIn(outIzq, digitalRead(outIzq) == HIGH ? LOW : HIGH);

  digitalWrite(s2Izq, HIGH);
  Green2 = pulseIn(outIzq, digitalRead(outIzq) == HIGH ? LOW : HIGH);
}

String DetectarColor(int R, int G, int B) {
  if (R <= 15 && G <= 15 && B <= 15)
    return "Blanco";
  else if (G < R && G < B && G < 23)
    return "Verde";
  else if (R < G && R < B && R < 23)
    return "Naranja";
  else
    return "Otro";
}

// ============================
// FUNCIONES DE MOTORES
// ============================
void StartMotor1() {
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
}

void StartMotor2() {
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
}

void StopMotor1() {
  motor1.run(RELEASE);
}

void StopMotor2() {
  motor2.run(RELEASE);
}

void StopMotors() {
  StopMotor1();
  StopMotor2();
}