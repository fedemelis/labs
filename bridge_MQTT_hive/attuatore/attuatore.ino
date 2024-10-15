void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()>1)
    { int val;
      val = Serial.read();
      if (val=='A') digitalWrite(2, HIGH);
      if (val=='S') digitalWrite(2, LOW);
    }

}
