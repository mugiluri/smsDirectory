/*------------------Pawa Tele Ltd--------------------*/
/*-------------------GSM SIM 800---------------------*/
/*----------------------Eris v1----------------------*/
//Email   : anthonyhost6@gmail.com
//Website : www.pawatele.com
// for #TheImpossibleHack || AfricasTalking
//Author  : Anthony Mugiluri
//Date    : 29th Aug 2020
//Time    : 11:01 EAT - Working Successfully
//Board   : STM_32~Eris v1
/*----------------------------------------------------*/

/*Connects to the reset pin of board 1(tophone)acts like an automatic push button
Receives the trigger signal from PB1 of board 1(tophone)
Connect the common ground to both SMS board(tophone) and reset board.
While programming, have the boot jumper wires capped.
You can detarch them after programming.
Watch the blinking light on the eris board. 
If it is off, press the reset button and wait.
Note that the purpose of this board is to reset the sms board thwice everytime it sends and sms.
To allow it to be able to respond to the next sms's automatically*/

#define RST PB1 //Connects to the reset pin of board 1(tophone)acts like an automatic push button
#define TRG PB5 //Receives the trigger signal from PB1 of board 1(tophone)
int count = 11; //Counter starts at 11 so that the board doesn't send the reset signal while initializing
int buttonState = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(RST, OUTPUT);
  pinMode(TRG, INPUT);
  digitalWrite(RST, LOW);
}

// the loop function runs over and over again forever
void loop() {
  buttonState = digitalRead(TRG);
  delay(200);
  count++;          
  if (buttonState == HIGH){
    count = 0;
    delay(200);//Every count is 200ms
    }
    
  if (count == 1){
    digitalWrite(RST, HIGH); 
    delay(400);                
    digitalWrite(RST, LOW);   
    }
    
   if (count == 10){
    digitalWrite(RST, HIGH);   
    delay(400);                   
    digitalWrite(RST, LOW);    
    }

   if (count >10){
    //Does nothing
    }
  
}
