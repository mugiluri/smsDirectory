#define TINY_GSM_MODEM_SIM800

#define WARN Serial.println

#define MQTTCLIENT_QOS2 1

#define APPLICATION_USERNAME "XXXXXX" 
#define DEVICE_GROUP_NAME "XXXXXX"
#define DEVICE_ID "Anyname"
#define DEVICE_GROUP_PASSWORD "XXXXXX"
#define MQTT_CREDENTIALS APPLICATION_USERNAME ":" DEVICE_GROUP_NAME
#define TOPIC_PREFIX APPLICATION_USERNAME "/" DEVICE_GROUP_NAME "/"

//#include "./include/config.h"

#include <Countdown.h>
#include <IPStack.h>
#include <MQTTClient.h>
#include <MQTTPacket.h>
#include <TinyGsmClient.h>
#include <DHT.h>
#include <Servo.h>

// BEGIN GSM CONFIG

const char apn[] = "iot.safaricom.com";
const char user[] = "";
const char pass[] = "";

/**
 * @brief Setting up the UART and initializing the GSM Client
 * 
 *  You need to be careful here. 
 *  Depending on the STM32 Core you're using you must be aware of the Serial Configuration.
 *  Consult the relevant docs if you can not see anything on the Serial.
 * 
 *  The GSM module power is at PB15 and needs to be set HIGH for 3 seconds then LOW once
 */
#define SerialMon Serial
#define SerialAT Serial2

TinyGsm modem(SerialAT);
TinyGsmClient tinyGSMClient(modem);

#define GSM_POWER_KEY PB15
#define RST PB1
int resetState = 0;
int count = 0;
int newCount = 0;

// END GSM CONFIG

// BEGIN MQTT CONFIG

const char mqttUsername[] = MQTT_CREDENTIALS;
const char mqttPassword[] = DEVICE_GROUP_PASSWORD;
const char mqttDeviceID[] = DEVICE_ID;

/**
 * @brief Setup Topics
 * 
 *  Notes on Topics: 
 * 
 *  birth topic - This is a topic that the device publishes to
 *  when it makes the first MQTT Connection or any other subsequent
 *  reconnections. It is not enforced but encouraged as a good practice.
 * 
 *  will topic - This is a topic that the device should publish to when 
 *  it disconnects. This can be used to detect your device going offline.
 *  It is sent together with the CONNECT payload.
 *  Also not enforced but considered a good practice.
 */
const char *birthTopic = TOPIC_PREFIX "birth";
const char *willTopic = TOPIC_PREFIX "will";
const char *servoTopic = TOPIC_PREFIX "servo";
const char *humidityTopic = TOPIC_PREFIX "humidity";
const char *soilMoistureTopic = TOPIC_PREFIX "moisture";
const char *temperatureTopic = TOPIC_PREFIX "temperature";
const char *ledTopic = TOPIC_PREFIX "led";
const char *lightIntensityTopic = TOPIC_PREFIX "logs";//Light
const char *ultraSonicDataTopic = TOPIC_PREFIX "distance";

const char birthMessage[] = "CONNECTED";
const char willMessage[] = "DISCONNECTED";

char brokerAddress[] = "broker.africastalking.com";
int brokerPort = 1883;

// END MQTT CONFIG

void brokerConnect(void);
void gsmConnect(void);
void incomingMessageHandler(MQTT::MessageData &messageData);

void sendTemperature(void);
void sendHumidity(void);
void sendLightIntensity(void);
void sendUltraSonicData(void);

IPStack ipstack(tinyGSMClient);
MQTT::Client<IPStack, Countdown, 128, 2> mqttClient = MQTT::Client<IPStack, Countdown, 128, 2>(ipstack);

char buffer[100];
int returnCode = 0;

#define TRIGGER_PIN PB10
#define ECHO_PIN PB11
#define DHT_PIN PA7//PB12
#define LDR_PIN PB12//PA7
#define SOIL_MOISTURE_PIN PB0
#define LED_PIN PC13
#define SERVO_PIN PB13

#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
Servo servo;

void setup()
{
    SerialMon.begin(115200);
    delay(100);
    SerialAT.begin(115200);
    delay(100);
    while (!SerialMon || !SerialAT)
    {
        ;
    }

    pinMode(RST, INPUT);

    pinMode(GSM_POWER_KEY, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(LDR_PIN, INPUT_ANALOG);
    pinMode(SOIL_MOISTURE_PIN, INPUT_ANALOG);
    pinMode(DHT_PIN, INPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIGGER_PIN, OUTPUT);

    digitalWrite(RST, LOW);

    // GSM ON
    digitalWrite(GSM_POWER_KEY, 1);
    delay(3000);
    digitalWrite(GSM_POWER_KEY, 0);

    gsmConnect();
    brokerConnect();
    delay(100);
    dht.begin();
    servo.attach(SERVO_PIN);
}

void loop()
{
    int count;
    resetState = digitalRead(RST);
    if (resetState == HIGH){
    newCount = count + 1;
    count = newCount;
    delay(400);
    }
 
    int isConnected = mqttClient.isConnected();
    if (!isConnected)
    {
        // Optionally you can call the gsmConnect() function
        modem.restart();
        brokerConnect();
    }
    else
    {
        sendTemperature();
        sendHumidity();
        //sendUltraSonicData();
        sendLightIntensity();
        mqttClient.yield(1000);
    }
    delay(5000);
}

void gsmConnect(void)
{
    sprintf(buffer, "Getting the modem ready \r\n");
    SerialMon.print(buffer);
    modem.restart();
    String modemInfo = modem.getModemInfo();
    SerialMon.print("About the modem: ");
    SerialMon.println(modemInfo);
    sprintf(buffer, "Initializing GSM network registration...\r\n");
    SerialMon.print(buffer);
    if (!modem.waitForNetwork())
    {
        sprintf(buffer, "\r\n Unable to initialize registration. Reset and try again.\r\n");
        SerialMon.print(buffer);
        modem.restart();
        while (true)
        {
            ;
        }
    }
    sprintf(buffer, "GSM OK\r\n");
    SerialMon.print(buffer);

    sprintf(buffer, "Attempting to establish GPRS connection \r\n");
    SerialMon.print(buffer);
    if (!modem.gprsConnect(apn, user, pass))
    {
        sprintf(buffer, "Unable to connect to APN. Reset and try again \r\n");
        SerialMon.print(buffer);
        while (true)
        {
            ;
        }
    }

    sprintf(buffer, "GSM is Okay \r\n");
    SerialMon.print(buffer);
}

void brokerConnect(void)
{
    MQTT::Message mqttMessage;
    snprintf(buffer, sizeof(buffer), "Connecting to %s on port %i \r\n", brokerAddress, brokerPort);
    returnCode = ipstack.connect(brokerAddress, brokerPort);
    SerialMon.println(buffer);
    if (returnCode != 1)
    {
        snprintf(buffer, sizeof(buffer), "Unable to connect to TCP Port. \r\n");
        SerialMon.println(buffer);
        while (true)
        {
            ;
        }
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "TCP port open \r\n");
        SerialMon.println(buffer);
    }
    // delay(3000);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 4;
    data.clientID.cstring = (char *)mqttDeviceID;
    data.username.cstring = (char *)mqttUsername;
    data.password.cstring = (char *)mqttPassword;
    data.keepAliveInterval = 60;
    data.cleansession = 1;
    data.will.message.cstring = (char *)willMessage;
    data.will.qos = MQTT::QOS1;
    data.will.retained = 0;
    data.will.topicName.cstring = (char *)willTopic;
    returnCode = mqttClient.connect(data);
    if (returnCode != 0)
    {
        snprintf(buffer, sizeof(buffer), "Code %i. Unable to contact the broker. Is it up? Reset and try again \r\n", returnCode);
        SerialMon.print(buffer);
        while (true)
        {
            ;
        }
    }
    mqttMessage.qos = MQTT::QOS1;
    mqttMessage.retained = false;
    mqttMessage.dup = false;
    mqttMessage.payload = (void *)birthMessage;
    mqttMessage.payloadlen = strlen(birthMessage) + 1;
    returnCode = mqttClient.publish(birthTopic, mqttMessage);
    snprintf(buffer, sizeof(buffer), "Birth topic publish return code %i \n", returnCode);
    SerialMon.println(buffer);
    returnCode = mqttClient.subscribe(servoTopic, MQTT::QOS1, incomingMessageHandler);
    if (returnCode != 0)
    {
        snprintf(buffer, sizeof(buffer), "Unable to subscribe to servo topic \r\n");
        SerialMon.print(buffer);
        while (true)
        {
            ;
        }
    }
    returnCode = mqttClient.subscribe(ledTopic, MQTT::QOS1, incomingMessageHandler);
    if (returnCode != 0)
    {
        snprintf(buffer, sizeof(buffer), "Unable to subscribe to LED topic \n");
        SerialMon.print(buffer);
        while (true)
        {
            ;
        }
    }
    snprintf(buffer, sizeof(buffer), "Successfully connected to the broker\n");
    SerialMon.println(buffer);
}

void publishMessage(char *payload, const char *topic)
{
    MQTT::Message message;
    message.qos = MQTT::QOS1;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload) + 1;
    returnCode = mqttClient.publish(topic, message);
    snprintf(buffer, sizeof(buffer), "%s topic publish return code %i \n", topic, returnCode);
    SerialMon.println(buffer);
}

void sendHumidity(void)
{
    float humidity = dht.readHumidity();
    snprintf(buffer, sizeof(buffer), "%.3f", humidity);
    publishMessage(buffer, humidityTopic);
}

void sendTemperature(void)
{
    float temperature = dht.readTemperature(false, false);
    snprintf(buffer, sizeof(buffer), "%.3f", temperature);
    publishMessage(buffer, temperatureTopic);
}

void sendLightIntensity(void)
{
    int count;
    //int intensity = analogRead(LDR_PIN);
    snprintf(buffer, sizeof(buffer), "%i", count);//intensity
    publishMessage(buffer, lightIntensityTopic);
}

void sendUltraSonicData(void)
{
    long duration, cm;
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH);
    cm = (duration / 2) / 29.1;
    snprintf(buffer, sizeof(buffer), "%lu", cm);
    publishMessage(buffer, ultraSonicDataTopic);
}

void incomingMessageHandler(MQTT::MessageData &messageData)
{
    MQTT::Message &message = messageData.message;
    MQTTString topic = messageData.topicName;
    snprintf(buffer, sizeof(buffer), "%s", messageData.topicName);
    if (buffer == servoTopic)
    {
        snprintf(buffer, sizeof(buffer), "%s", messageData.message.payload);
        if (buffer == "open")
        {
            servo.write(180);
        }
        else if (buffer == "close")
        {
            servo.write(0);
        }
        else
        {
            SerialMon.print("Unknown Servo Command: ");
            SerialMon.println(buffer);
        }
    }
    else if (buffer == ledTopic)
    {
        snprintf(buffer, sizeof(buffer), "%s", messageData.message.payload);
        if (buffer == "on")
        {
            digitalWrite(LED_PIN, 1);
        }
        else if (buffer == "off")
        {
            digitalWrite(LED_PIN, 0);
        }
        else
        {
            SerialMon.print("Unknown LED Command: ");
            SerialMon.println(buffer);
        }
    }
    else
    {
        SerialMon.print("Unknown Topic: ");
        SerialMon.println(buffer);
    }
}
