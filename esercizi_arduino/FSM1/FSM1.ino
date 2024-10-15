// FSM - version 1

byte iCurrentState;
// States:
// A: 0, B: 1, C:2, D:3
// inputs:
// P(ressed):1 R(eleased): 0

#define INPUTPIN (2)
#define OUTPUTPIN (4)


void setup(){
  iCurrentState = 0; // set the initial state
  pinMode(OUTPUTPIN , OUTPUT);
  pinMode(INPUTPIN , INPUT);
}


void loop(){

   //1. read the input
   byte iInput;

   //2. convert it into the input symbol
   if (digitalRead(INPUTPIN )==HIGH) 
	{iInput = 0;}
   else 
	{iInput = 1;}
   
      
   //3. Function F (future state) (decide witch state is the future state using current state and input)
    byte iFutureState;

   iFutureState = iCurrentState; // default: same state
   if ((iCurrentState==0) && (iInput==1)) iFutureState=1; // A->B
   if ((iCurrentState==1) && (iInput==0)) iFutureState=2; 
   if ((iCurrentState==2) && (iInput==1)) iFutureState=3; 
   if ((iCurrentState==3) && (iInput==0)) iFutureState=0; 
   
   //4. state transition
   iCurrentState= iFutureState;
   
   //5. function G (sending output based on current state)
   if (iCurrentState==0) digitalWrite(OUTPUTPIN , LOW);  // A
   if (iCurrentState==1) digitalWrite(OUTPUTPIN , HIGH); // B
   if (iCurrentState==2) digitalWrite(OUTPUTPIN , HIGH); // C
   if (iCurrentState==3) digitalWrite(OUTPUTPIN , LOW);  // D
   
   
}