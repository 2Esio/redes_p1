// D2 a D9 representan TX1 a TX8; el cableado determina el LED receptor.
const int salidas[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(salidas[i], OUTPUT);
    digitalWrite(salidas[i], LOW);
  }
}

// El barrido es igual para ambos cables: el cruce se realiza en el circuito.
void loop() {
  for (int i = 0; i < 8; i++) {
    Serial.print("TX ");
    Serial.println(i + 1);
    // Pulso de 700 ms y pausa de 200 ms para distinguir los conductores.
    digitalWrite(salidas[i], HIGH);
    delay(700);
    digitalWrite(salidas[i], LOW);
    delay(200);
  }
  Serial.println("Fin del ciclo");
  delay(1000);
}
