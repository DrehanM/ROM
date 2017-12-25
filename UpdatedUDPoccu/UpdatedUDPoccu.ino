#include <SoftwareSerial.h>

SoftwareSerial esp8266(11,12); // RX, TX
int IR_0 = 0;
int IR_1 = 2;
int val0 = 0;
int val1 = 0;
int count = 0; //Occupancy

// For autonomous threshold calculation
int high0 = 0;
int high1 = 0;
int set_val0 = 0;
int set_val1 = 0;

// To track if the second IR beam is tripped in time
int tracker = 0;

// Variables to operate with connectivity thread
int msgindx = 0;
char msgBuffer[64]; //Stores the packet
int msgLen = 0;
char commandStr[64]; //Specifies size of packet

// Time storage for the timer to allow double threading of sensing and packet transmission
unsigned long currentTime;
unsigned long tsl; //Time Sent Last of previous packet

void setup() {
  Serial.begin(9600);
  tsl = millis();

  delay(1000);
  
  //To autonomously determine the correct threshold for detection
  high0 = analogRead(IR_0);
  high1 = analogRead(IR_1);
  while (millis() < 10000){
    set_val0 = analogRead(IR_0);
    set_val1 = analogRead(IR_1);
    if (set_val0 > high0 and set_val0 < 300){
      high0 = set_val0; 
    }
    if (set_val1 > high1 and set_val1 < 300){
      high1 = set_val1;
    }
  }
  
  Serial.print("IR_0 high : ");
  Serial.println(high0);
  Serial.print("IR_1 high : ");
  Serial.println(high1);
  
  esp8266.begin(19200);
  esp8266.setTimeout(10000);

  //reset WLAN
  esp8266.println("AT+RST");
  if (!esp8266.findUntil("ready", "ERROR")){
    Serial.println("ESP ERROR: AT+RST failed");
  }

  //config WLAN
  esp8266.println("AT+CWMODE=1");
  if (!esp8266.findUntil("OK", "ERROR")){
    Serial.println("ESP ERROR: AT+CWMODE=1 failed");
  }

  //Change the SSID and Password to that of the network being used
  esp8266.println("AT+CWJAP=\"CN-Students\",\"CnStudentsWiFiAccess123\"");
  if (!esp8266.findUntil("OK", "ERROR")){
    Serial.println("ESP ERROR: AT+CWJAP=\"Nomad_EXT\"... failed");
  }

  //get IP
  esp8266.println("AT+CIFSR");
  if (!esp8266.findUntil("OK", "ERROR")){
    Serial.println("ESP ERROR: AT+CIFSR");
  }

  //set up UDP connection
  esp8266.println("AT+CIPSTART=\"UDP\",\"192.168.0.245\",9999,8888,0");
  if (!esp8266.findUntil("OK", "ERROR")){
    Serial.println("ESP ERROR: AT+CIPSTART failed");
  }
}

void loop() {

  //Read sensor values
  val0 = analogRead(IR_0);
  val1 = analogRead(IR_1);
  currentTime = millis();

  //Loop for detection of exit
  while (val0 > high0+10 and val1 < high1+10){
    delay(10);
    tracker++; 
    val1 = analogRead(IR_1);
    if (val0 > high0+10 and val1 > high1+10){
        Serial.print("Detected exit: ");
        count-=1;
        Serial.println(count);
        delay(500); //necessary delay for reset
    }
    //Tracks for 0.5 seconds
    if (tracker > 50){
      tracker = 0;
      break;         
    }
  }

  //Loop for detection of entry
  while (val1 > high1+10 and val0 < high0+10){
    delay(10);
    tracker++;
    val0 = analogRead(IR_0);
    if (val1 > high1+10 and val0 > high0+10){
        Serial.print("Detected entry: ");
        count+=1;
        Serial.println(count);
        delay(500); //necessary delay for reset
    }
    //Tracks for 0.5 seconds
    if (tracker > 50){
        tracker = 0;
        break;
    }
  }
 
//Enters packet transmission if 5 seconds have passed since the previous transmission
if (currentTime > tsl + 5000){
  tsl = millis();
  sprintf(msgBuffer, "%d,%d", msgindx, count);
  msgLen = strlen(msgBuffer);
  sprintf(commandStr, "AT+CIPSEND=%d", msgLen);
  esp8266.println(commandStr);
  delay(200); //necessary delay to allow specify packet size
  msgindx++;
  esp8266.write(msgBuffer);
  Serial.println("Sent: UDP");
  Serial.println(msgBuffer);
}
  

  
  delay(100);
}

