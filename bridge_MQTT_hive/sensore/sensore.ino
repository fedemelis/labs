
unsigned long timestamp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  timestamp = millis();
  
}

void loop() {
  int val1;
  int val2;
  if (millis() - timestamp > 1000){
    val1 = analogRead(0);
    val2 = analogRead(1);
    // pacchetto dati
    // FF  2  dato1 dato2 FE
    Serial.write(0xFF);
    Serial.write(2);
    
    Serial.write(map(val1,0,1023,0,253));
    Serial.write(map(val2,0,1023,0,253));

    Serial.write(0xFE);
    
    timestamp = millis();
  }
  
}
