#define RST PB1
#define TRG PB5
int count = 11;
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
