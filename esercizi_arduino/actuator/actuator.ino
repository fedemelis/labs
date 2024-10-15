enum State{
  A, B, C, D
};

State currentState = A;
State futureState = A;
uint64_t startmillis = 0;

bool timeout(uint64_t duration){
  if((millis() - startmillis) > duration){
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(2,OUTPUT);
}

void loop() {
  if (Serial.available()) {
      // reading input
      int input;
      input = Serial.read();
      // state transition
      futureState = currentState;
      if((currentState == A) && (input == 'A')){
        futureState = B;
      }
      if((currentState == A) && (input == 'B')){
        futureState = C;
      }
      if((currentState == B) && (input == 'S')){
        futureState = A;
      }
      if((currentState == B) && (input == 'B')){
        futureState = C;
      }
      if((currentState == C) && (input == 'A')){
        futureState = B;
      }
      if((currentState == C) && (input == 'S')){
        futureState = A;
      }
      if((currentState == D) && (input == 'S')){
        futureState = A;
      }
      if((currentState == D) && (input == 'A')){
        futureState = B;
      }
    }
    if((currentState == C) && (timeout(300))){
        futureState = D;
    }
    if((currentState == D) && (timeout(300))){
        futureState = C;
    }
  //on-exit function
    if(currentState != futureState){

    }

    // on-entry action
    if(currentState != futureState){
      if((futureState == C) || (futureState == D)){
       startmillis = millis();
      }
    }

    if(currentState != futureState){
      currentState = futureState;
      
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

