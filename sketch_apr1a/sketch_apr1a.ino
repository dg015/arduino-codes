// C++ code
//

//card reader
#include <SPI.h>
#include <MFRC522.h>

//button
const int buttonPin = 7;
bool buttonState = false;

//switch (tap)
const int switchPin = 8;
bool switchState = false;


//RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String cardUID = "";
//Light sensor Light pin is using ANALOG
const float GAMMA = 0.7;
const float RL10 = 50;

unsigned long lastCardRead = 0;
const int cardTimeout = 500; // milliseconds

//coasters
float coasterValue1;
float coasterValue2;
float coasterValue3;


#define LDR_PIN 2

// unity comunicaiton
unsigned long lastSend = 0;
const int sendInterval = 50; // 20Hz

/*
// colour detection
#define s0 8
#define s1 9
#define s2 10
#define s3 11
#define out 12
int   Red=0, Blue=0, Green=0;  //RGB values
String currentColour;
*/


void setup()
{
  Serial.begin(115200);

  //Send button
  pinMode(buttonPin, INPUT_PULLUP);

  // beer tap
  pinMode(switchPin, INPUT_PULLUP);

/*
  //colour detection pin setup
  pinMode(s0,OUTPUT);    
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);

  digitalWrite(s0,HIGH);
  digitalWrite(s1,LOW);

*/

  //RFID card
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
}





void loop()
{
  checkButtonSubmitDrink();
  checkSwitchState();
  readCard();

  //coaster
  checkLightCoaster(A0,coasterValue1);
  checkLightCoaster(A1,coasterValue2);
  checkLightCoaster(A2,coasterValue3);
  //getColour();

    if (millis() - lastSend > sendInterval)
  {
    lastSend = millis();
    gatherData();
  }
}

/*
void getColour()
{
GetColors();

  Serial.print("R=");
  Serial.print(Red);
  Serial.print(" G=");
  Serial.print(Green);
  Serial.print(" B=");
  Serial.println(Blue);

  int threshold = 10; // noise tolerance

  // RED detection (Red is clearly lower than others)
  if (Red < Green - threshold && Red < Blue - threshold)
  {
    currentColour = "Red";
    Serial.println("Red");
  }
  // GREEN detection
  else if (Green < Red - threshold && Green < Blue - threshold)
  {
    currentColour = "Green";
    Serial.println("Green");
  }
  // BLUE detection
  else if (Blue < Red - threshold && Blue < Green - threshold)
  {
    currentColour = "Blue";
    Serial.println("Blue");
  }
  // everything else (white / unclear surface)
  else
  {
    currentColour = "Unknown";
    Serial.println("Unknown");
  }

  delay(500);
}

*/





void readCard()
{
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    // clear only after timeout
    if (millis() - lastCardRead > cardTimeout)
    {
      cardUID = "";
    }

    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  // remember last successful read time
  lastCardRead = millis();

  //Show UID on serial monitor
  cardUID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }

  //set content to upper case
  cardUID.toUpperCase();
} 


void checkLightCoaster(int pin, float &coasterValue)
{
  /*
    int analogValue = analogRead(pin);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  Serial.println(lux);
  */
  int analogValue = analogRead(pin);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  coasterValue = lux;
  //Serial.println(lux);


}

//MISSING COLOUR SENSOR
//will pass the data to the arduino
void gatherData()
{
  //data order COASTERS -> BUTTON -> SWITCH -> RFID
  //coaster data
  Serial.print((int)coasterValue1);
  Serial.print(",");
  Serial.print((int)coasterValue2);
  Serial.print(",");
  Serial.print((int)coasterValue3);
  Serial.print(",");

  //button data
  Serial.print(buttonState);
  Serial.print(",");

  // Switch data
  Serial.print(switchState);
  Serial.print(",");
  
  // just in case its null so it sends none
  String safeUID = cardUID;
  safeUID.replace(",", ""); 
  Serial.println(safeUID.length() > 0 ? safeUID : "NONE");
}


void checkButtonSubmitDrink()
{

  buttonState = digitalRead(buttonPin);
  // NOT PRESSINGS
  if(buttonState == HIGH)
  {
    //Serial.println("pressed");
  }
  //PRESSING
  else if (buttonState == LOW)
  {
    //Serial.println("NOT pressed");
  }

}


void checkSwitchState()
{

  switchState = digitalRead(switchPin);
  // NOT PRESSINGS
  if(switchState == HIGH)
  {
   // Serial.println("pressed");
  }
  //PRESSING
  else if (switchState == LOW)
  {
    //Serial.println("NOT pressed");
  }
}
