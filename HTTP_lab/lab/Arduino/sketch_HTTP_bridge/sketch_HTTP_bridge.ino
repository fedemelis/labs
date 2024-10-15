
unsigned long timestamp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  timestamp = millis();
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
    { int dato;
      dato = Serial.read();
      if (dato=='A') digitalWrite(2, HIGH);
      if (dato=='S') digitalWrite(2, LOW);
    }

  int dato1;
  int dato2;
  if (millis() - timestamp > 5000){
    dato1 = analogRead(0);
    // pacchetto dati
    // FF  1  dato1 FE
    
    Serial.write(0xFF);

    Serial.write(1);
    
    Serial.write(map(dato1,0,1023,0,253));
  
    Serial.write(0xFE);
    
    timestamp = millis();
  }
}