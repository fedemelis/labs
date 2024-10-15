  enum State {
  A, B, C, D, E
};

uint64_t startMillis;
State currentState = A;
State futureState = A;

bool timeout(uint64_t duration){
  return ((millis() - startMillis) > duration);
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);  
}

void loop() {
  uint8_t input;
  uint8_t inputFSM;

  // reading input
  input=digitalRead(2);

  //Serial.println(input);

  // converting input in input symbol for FSM
  if(input == HIGH){
    inputFSM = 1;
  }else{
    inputFSM = 0;
  }


  // changing states
  futureState = currentState; // we saying that the default behaviour is the loop, so we can write only the changes of state
  if((currentState == A) && (inputFSM == 1)){
    futureState = B;
  }
  if((currentState == B) && (inputFSM == 0)){
    futureState = C;
  }
  if((currentState == C) && (inputFSM == 1)){
    futureState = E;
  }
  if((currentState == C) && (timeout(2000))){
    futureState = D;
  }
  if((currentState == D) && (timeout(2000))){
    futureState = C;
  }
  if((currentState == D) && (inputFSM == 1)){
    futureState = E;
  }
  if((currentState == E) && (inputFSM == 0)){
    futureState = A;
  }

  /*
  some kind of on-exit actions
  */

  //on-entry action
  if(currentState != futureState){
    if(futureState == C || futureState == D){
      startMillis = millis();
    }
  }

  currentState = futureState;
  //Serial.println(currentState);
  if(currentState == A){
    digitalWrite(13, LOW);
  }
  if(currentState == B){
    digitalWrite(13, HIGH);
  }
  if(currentState == C){
    digitalWrite(13, HIGH);
  }
  if(currentState == D){
    digitalWrite(13, LOW);
  }
  if(currentState == E){
    digitalWrite(13, LOW);
  }
  delay(20);
}