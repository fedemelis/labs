// FSM - version 3 with max duration


// States:
enum State {StateA, StateB, StateC_ON, StateC_OFF, StateD};
// inputs:
enum Input {PRESSED, RELEASED};
// outputs:
enum Output {ON, OFF};

State iCurrentState;
unsigned long lastTime;

#define INPUTPIN (2)
#define OUTPUTPIN (4)

// blink frequency: 2 Hz  -> 250ms HIGH, 250ms LOW
#define BLINKTIME  (250) 

void setup(){
  iCurrentState = StateA; // set the initial state

  pinMode(OUTPUTPIN , OUTPUT);
  pinMode(INPUTPIN , INPUT);
  lastTime = millis();
}


void loop(){

   // read the input and convert it into the input symbol
   Input iInput;
   if (digitalRead(INPUTPIN)==HIGH) 
	{iInput = RELEASED;}
   else 
	{iInput = PRESSED;}
   
      
   // Function F (future state)
   State iFutureState;

   iFutureState = iCurrentState; // default: same state
   if ((iCurrentState==StateA) && (iInput==PRESSED)) iFutureState = StateB; 
   if ((iCurrentState==StateB) && (iInput==RELEASED)) iFutureState = StateC_ON; 
   if ((iCurrentState==StateC_ON) && (iInput==PRESSED)) iFutureState = StateD; 
   if ((iCurrentState==StateC_OFF) && (iInput==PRESSED)) iFutureState = StateD; 
   if ((iCurrentState==StateD) && (iInput==RELEASED)) iFutureState = StateA; 

   if ((iCurrentState==StateC_ON) && (millis()-lastTime > BLINKTIME)) iFutureState = StateC_OFF; 
   if ((iCurrentState==StateC_OFF) && (millis()-lastTime > BLINKTIME)) iFutureState = StateC_ON; 
   
   // onentry and onexit actions 
   if (	iCurrentState != iFutureState){
        lastTime = millis();  // set the time of each transition
   }

   // state transition
   iCurrentState= iFutureState;
   
   // function G
   Output output;
   if (iCurrentState==StateA || iCurrentState==StateD || iCurrentState==StateC_OFF ) output = OFF; 
   if (iCurrentState==StateB || iCurrentState==StateC_ON ) output = ON;  

   //  output symbols -> actuators
   if (output==ON) digitalWrite(OUTPUTPIN , HIGH);  
   if (output==OFF) digitalWrite(OUTPUTPIN , LOW);  

   
   
}
