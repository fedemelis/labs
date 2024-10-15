enum State{
  A, B, C, D
};

State currentState;
State futureState;
uint64_t startmillis = 0;

bool timeout(uint64_t duration){
  return ((millis() - startmillis) > duration);
}

void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  currentState = A;
  futureState = A;
}

void loop() {
  int input = 0;
  if (Serial.available() > 0) {
      // reading input
      input = Serial.read();

      // state transition
      futureState = currentState;
      if(currentState == A && input == 'A'){
        futureState = B;
      }
      if(currentState == A && input == 'B'){
        futureState = C;
      }
      if(currentState == B && input == 'S'){
        futureState = A;
      }
      if(currentState == B && input == 'B'){
        futureState = C;
      }
      if(currentState == C && input == 'A'){
        futureState = B;
      }
      if(currentState == C && input == 'S'){
        futureState = A;
      }
      if(currentState == C && timeout(500)){
        futureState = D;
      }
      if(currentState == D && input == 'S'){
        futureState = A;
      }
      if(currentState == D && input == 'A'){
        futureState = B;
      }
      if(currentState == D && timeout(500)){
        futureState = C;
      }

      // on-exit function
      if(currentState != futureState){

      }

      // on-entry action
      if(currentState != futureState){
        if(futureState == 'C' || futureState == 'D'){
          startmillis = millis();
        }
      }

      currentState = futureState;
      
      //output
      if(currentState == A){
        digitalWrite(2, LOW);
      }
      if(currentState == B){
        digitalWrite(2, HIGH);
      }
      if(currentState == C){
        digitalWrite(2, LOW);
      }
      if(currentState == D){
        digitalWrite(2, HIGH);
      }
    }
}

