// FSM - version 2 with enum

/*
con gli enum ci aiutiamo perché diamo dei nomi parlanti agli stati
e alle variabili di input e output
*/

// States:
enum State {StateA, StateB, StateC, StateD};
// inputs:
enum Input {PRESSED, RELEASED};
// outputs:
enum Output {ON, OFF};

State iCurrentState;

#define INPUTPIN (2)
#define OUTPUTPIN (4)

void setup(){
  iCurrentState = StateA; // set the initial state

  pinMode(OUTPUTPIN , OUTPUT);
  pinMode(INPUTPIN , INPUT);
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
   if ((iCurrentState==StateB) && (iInput==RELEASED)) iFutureState = StateC; 
   if ((iCurrentState==StateC) && (iInput==PRESSED)) iFutureState = StateD; 
   if ((iCurrentState==StateD) && (iInput==RELEASED)) iFutureState = StateA; 
   
   // state transition
   iCurrentState= iFutureState;
   
   // function G
   Output output;
   if (iCurrentState==StateA || iCurrentState==StateD ) output = OFF; 
   if (iCurrentState==StateB || iCurrentState==StateC ) output = ON;  

   //  output symbols -> actuators
   if (output==ON) digitalWrite(OUTPUTPIN , HIGH);  
   if (output==OFF) digitalWrite(OUTPUTPIN , LOW);  

   
   
}
