// FSM - version 3 with max duration


// States:
enum State {StateA, StateB, StateC, StateD};
// inputs:
enum Input {PRESSED, RELEASED};
// outputs:
enum Output {ON, OFF};

State iCurrentState;
unsigned long lastTime;

#define INPUTPIN (2)
#define OUTPUTPIN (4)

// 2seconds
#define MAXTIME  (2000) 

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
   if ((iCurrentState==StateB) && (iInput==RELEASED)) iFutureState = StateC; 
   if ((iCurrentState==StateC) && (iInput==PRESSED)) iFutureState = StateD; 
   if ((iCurrentState==StateD) && (iInput==RELEASED)) iFutureState = StateA; 
   if ((iCurrentState==StateC) && (millis()-lastTime > MAXTIME)) iFutureState = StateA; 

   /*
   NB: abbiamo modellato il timer come un passaggio di stato, poiché effettivamente è un arco della FSM
   */
   
   // onentry and onexit actions 
   if (	iCurrentState != iFutureState){
      if (iFutureState ==StateC)  // entering state C
        lastTime = millis();

   }

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
