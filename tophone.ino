/*------------------Pawa Tele Ltd--------------------*/
/*-------------------GSM SIM 800---------------------*/
/*----------------------Eris v1----------------------*/
//Email   : anthonyhost6@gmail.com
//Website : www.pawatele.com
// for #TheImpossibleHack || AfricasTalking
//Author  : Anthony Mugiluri
//Date    : 28th Aug 2020
//Time    : 11:01 EAT - Working Successfully
//Board   : STM_32~Eris v1
//Companion: Reset file to receive the reset trigger is needed(2 Eris boards needed to run this project successfully)

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <DHT.h>

#define WARN Serial.println
#define SerialMon Serial
#define SerialAT Serial2


#define GSM_POWER_KEY PB15
#define TRG PB1
#define DHT_PIN PA7
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);


TinyGsm modem(SerialAT);
TinyGsmClient tinyGSMClient(modem);

String Mynumber = "\"+2547XXXXXXXX\""; //Default registered number to pataNairobi{Place holder}
String msg = String(""); // String buffer for the GPRS shield message (Body)
String mynum = String(""); // String buffer for the GPRS shield message (Number)
int SmsContentFlag = 0; // Set to 1 when the next GPRS shield message will contains the SMS message

void setup() {

pinMode(GSM_POWER_KEY, OUTPUT);
pinMode(TRG, OUTPUT);
pinMode(DHT_PIN, INPUT);
digitalWrite( TRG, LOW);

SerialMon.begin(115200);
    delay(100);
    SerialAT.begin(115200);
    delay(100);
    while (!SerialMon || !SerialAT)
    {
        ;
    }

     //GSM ON
    digitalWrite(GSM_POWER_KEY, 1);
    delay(3000);
    digitalWrite(GSM_POWER_KEY, 0);
    dht.begin();

SerialAT.begin(115200); //Begin serial communication with Arduino and SIM800
Serial.println("Starting...");
delay(5000);
Serial.println("Setup Complete!");
SerialAT.println("AT");
delay(200);
SerialAT.println("AT+CMGF=1");
delay(200);
SerialAT.println("AT+CNMI=1,2,0,0,0"); //set module to read live messages +CMT:
delay(200);
}

void loop() {
if(SerialMon.available()){ //Read Arduino IDE Serial Monitor inputs (if available) and send them to SIM800//Serial Monitor
SerialAT.write(SerialMon.read());//Serial Monitor
}
char SerialInByte;
if(SerialAT.available())
{ 
SerialInByte = (unsigned char)SerialAT.read();
SerialMon.print(SerialInByte);//Serial Monitor

delay(5);

// -------------------------------------------------------------------
// Program also listen to the GPRS shield message.
// -------------------------------------------------------------------
// If the message ends with <CR> then process the message
if( SerialInByte == 13 ){//13
// Store the char into the message buffer
ProcessGprsMsg();
}
if( SerialInByte == 10 ){//10
// Skip Line feed
}
else {
msg += String(SerialInByte); // store the current character in the message string buffer
}
}

}
// Make action based on the content of the SMS. 
// Notice than SMS content is the result of the processing of several GPRS shield messages.
void ProcessSms( String sms ){

if( sms.indexOf("hello") >= 0 || sms.indexOf("Hello") >= 0 || sms.indexOf("HOW") >= 0 || sms.indexOf("how") >= 0 || sms.indexOf("Hi") >= 0 || sms.indexOf("HI") >= 0 ){
  
  Mynumber = mynum; 
  response("Hi,thank you for contacting pataNairobi. How can I help you?");
}

if( sms.indexOf("Cool") >= 0 || sms.indexOf("cool") >= 0 || sms.indexOf("Good") >= 0 || sms.indexOf("good") >= 0 || sms.indexOf("Nice") >= 0 || sms.indexOf("nice") >= 0 || sms.indexOf("Awesome") >= 0 || sms.indexOf("awesome") >= 0 || sms.indexOf("Fantastic") >= 0 || sms.indexOf("fantastic") >= 0 || sms.indexOf("Great") >= 0 || sms.indexOf("great") >= 0 ){
  
  Mynumber = mynum; 
  response("Thank you for appreciating. Karibu!");
}

if( sms.indexOf("improve") >= 0 || sms.indexOf("Un") >= 0 || sms.indexOf("un") >= 0 || sms.indexOf("Not") >= 0 || sms.indexOf("not") >= 0 || sms.indexOf("don't") >= 0 || sms.indexOf("Bad") >= 0 || sms.indexOf("bad") >= 0 ){
  
  Mynumber = mynum; 
  response("We are very sorry and we are improving on it. Karibu!");
}

if( sms.indexOf("Bye") >= 0 || sms.indexOf("bye") >= 0 || sms.indexOf("BYE") >= 0 || sms.indexOf("See") >= 0 || sms.indexOf("see") >= 0 || sms.indexOf("SEE") >= 0 || sms.indexOf("YOU") >= 0 || sms.indexOf("you") >= 0 || sms.indexOf("another") >= 0 || sms.indexOf("ANOTHER") >= 0 || sms.indexOf("DAY") >= 0 || sms.indexOf("day") >= 0 || sms.indexOf("Have") >= 0 || sms.indexOf("HAVE") >= 0 || sms.indexOf("have") >= 0 ){
  
  Mynumber = mynum; 
  response("Bye friend! See you next time at pataNairobi");
}

if( sms.indexOf("Temperature") >= 0 || sms.indexOf("temperature") >= 0 || sms.indexOf("TEMPERATURE") >= 0 || sms.indexOf("Hot") >= 0 || sms.indexOf("HOT") >= 0 || sms.indexOf("hot") >= 0 ){
  
  Mynumber = mynum; 
  sendTemperature();
}

if( sms.indexOf("Humidity") >= 0 || sms.indexOf("humidity") >= 0 || sms.indexOf("HUMIDITY") >= 0 || sms.indexOf("humid") >= 0 || sms.indexOf("mist") >= 0 || sms.indexOf("fog") >= 0  || sms.indexOf("Mist") >= 0 || sms.indexOf("Fog") >= 0 ){
  
  Mynumber = mynum; 
  sendHumidity();
}
if( sms.indexOf("Africastalking") >= 0 || sms.indexOf("AFRICASTALKING") >= 0 || sms.indexOf("africastalking") >= 0 || sms.indexOf("Africas talking") >= 0 || sms.indexOf("africas talking") >= 0 ){
  
  Mynumber = mynum; 
  response("Mbabane Rd, Nairobi, Phone 0729 891801");
}

if( sms.indexOf("IHub") >= 0 || sms.indexOf("IHUB") >= 0 || sms.indexOf("ihub") >= 0 || sms.indexOf("Ihub") >= 0 || sms.indexOf("I Hub") >= 0 ){
  
  Mynumber = mynum; 
  response("Kilimani Ngong rd Senteu Plaza, Nairobi, Founder: Erik Hersman CEO: Bosun Tijani Phone: 0733 369010");
}
if( sms.indexOf("NSSF") >= 0 || sms.indexOf("nssf") >= 0 || sms.indexOf("Nssf") >= 0 ){
  
  Mynumber = mynum; 
  response("Westlands: 0784398348438, Lavington: 98893488934 Community: 898438384348 City Center: 073898277273");
}

if( sms.indexOf("NHIF") >= 0 || sms.indexOf("nhif") >= 0 || sms.indexOf("Nhif") >= 0 ){
  
  Mynumber = mynum; 
  response("Westlands: 0784398348438, Lavington: 98893488934 Community: 898438384348 City Center: 073898277273");
}

if( sms.indexOf("Web designer") >= 0 || sms.indexOf("Webdesigner") >= 0 || sms.indexOf("Web Designer") >= 0 || sms.indexOf("Programmer") >= 0 || sms.indexOf("programmer") >= 0  || sms.indexOf("Code") >= 0 || sms.indexOf("code") >= 0 || sms.indexOf("Developer") >= 0   || sms.indexOf("developer") >= 0 || sms.indexOf("Dev") >= 0 || sms.indexOf("dev") >= 0 ){
  
  Mynumber = mynum; 
  response("Asha{Python}: 0784398348438, Brian{Java}: 98893488934 Ruth{css/html}: 898438384348 Peter{Node js}: 073898277273");
}

Mynumber = mynum;
response("Every opinion is highly valued at pataNairobi. Thank you and Karibu!");

}

void GprsTextModeSMS(){
Serial.println( "AT+CMGF=1" ); // Request Text Mode for SMS messaging
}

void GprsReadSmsStore( String SmsStorePos ){
SerialMon.print( "AT+CMGR=" );
SerialMon.println( SmsStorePos );
delay(200);
SerialMon.print( "AT+CMGD=" ); //Deletes SMS from sim once read
SerialMon.println( SmsStorePos );
}

// Clear the GPRS shield message buffer
void ClearGprsMsg(){
msg = "";
}

// EN: interpret the GPRS shield message and act appropiately
void ProcessGprsMsg() {
if( msg.indexOf( "Call Ready" ) >= 0 ){
Serial.println( "*** GPRS Shield registered on Mobile Network ***" );
GprsTextModeSMS();
}

// unsolicited message received when getting a SMS message
if( msg.indexOf( "+CMTI" ) >= 0 ){
 Serial.println( "*** SMS Received ***" );
// Look for the coma in the full message (+CMTI: "SM",6)
// In the sample, the SMS is stored at position 6
int iPos = msg.indexOf( "," );
String SmsStorePos = msg.substring( iPos+1 );
 Serial.print( "SMS stored at " );
 Serial.println( SmsStorePos ); 
// Ask to read the SMS store
GprsReadSmsStore( SmsStorePos );
}

//Extracting the number
if( msg.indexOf( "+CMT" ) >= 0 ){
 Serial.println("");
 Serial.println( "*** SMS Received ***" );
// Look for the plus in the full message (+CMTI: "SM",6)
// In the sample, the SMS is stored at position 6
int iPos1 = msg.indexOf( "+" );
int i = iPos1+2;
String SmsStorePos1 = msg.substring(i);

Serial.print( "Client's number is " );
for(i; i < iPos1+18; i++)
    {
      Serial.print( SmsStorePos1[i] );
      mynum += String(SmsStorePos1[i]);
    }
}

// SMS store readed through UART (result of GprsReadSmsStore request) 
if( msg.indexOf( "+CMGR:" ) >= 0 ){
// Next message will contains the BODY of SMS
SmsContentFlag = 1;
// Following lines are essentiel to not clear the flag!
ClearGprsMsg();
return;
}

if( msg.indexOf( "+CMT:" ) >= 0 ){
// Next message will contains the BODY of SMS
SmsContentFlag = 1;
// Following lines are essentiel to not clear the flag!
ClearGprsMsg();
return;
}

// +CMGR message just before indicate that the following GRPS Shield message 
// (this message) will contains the SMS body 
if( SmsContentFlag == 1 ){
 Serial.println( "--From Phone to Pawa Tele SED--" );
 Serial.println( "*** SMS MESSAGE CONTENT ***" );
 Serial.println( msg );
 Serial.println( "*** END OF SMS MESSAGE ***" );
ProcessSms( msg );
}

ClearGprsMsg(); // Always clear the flag
SmsContentFlag = 0; 

}

void refresh(){
    digitalWrite(TRG, 1);
    delay(400);
    digitalWrite(TRG, 0);
  }   


void response(String text){
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.print("\r\n");
      
      delay(500);
      String Mynumber1 = "AT+CMGS="+Mynumber;
      SerialAT.println(Mynumber1);
      delay(500);
      SerialAT.println(text);//Text to send
      SerialAT.write(0x1a); //ctrl + z
      SerialAT.println("AT+CMGL=\"ALL\""); //List all messages

      
      delay(100);
      refresh();
      
      }
      
void sendHumidity(void)
{
    float humidity = dht.readHumidity();
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.print("\r\n");
      
      delay(500);
      String Mynumber1 = "AT+CMGS="+Mynumber;
      SerialAT.println(Mynumber1);
      delay(500);
      SerialAT.print("Humidity in Nairobi is ");
      SerialAT.print(humidity);
      SerialAT.println("RH%.");
      SerialAT.write(0x1a); //ctrl + z
      SerialAT.println("AT+CMGL=\"ALL\""); //List all messages
      delay(100);
      refresh();
}

void sendTemperature(void)
{
    float temperature = dht.readTemperature(false, false);
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.write(27);
      SerialAT.print("\r\n");
      
      delay(500);
      String Mynumber1 = "AT+CMGS="+Mynumber;
      SerialAT.println(Mynumber1);
      delay(500);
      SerialAT.print("Temperature in Nairobi is ");
      SerialAT.print(temperature);
      SerialAT.println("°C");
      SerialAT.write(0x1a); //ctrl + z
      SerialAT.println("AT+CMGL=\"ALL\""); //List all messages
      delay(100);
      refresh();
}
