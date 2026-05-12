// C++ code
//

//card reader
#include <SPI.h>
#include <MFRC522.h>

//button
const int buttonPin = 5;
bool buttonState = false;

//switch (tap)
const int switchPin = 4;
bool switchState = false;


//RFID
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


//Light sensor Light pin is using ANALOG
const float GAMMA = 0.7;
const float RL10 = 50;

//coasters
float coasterValue1;
float coasterValue2;
float coasterValue3;


#define LDR_PIN 2


void setup()
{
  Serial.begin(115200);

  //Send button
  pinMode(buttonPin, INPUT_PULLUP);

  // beer tap
  pinMode(switchPin, INPUT_PULLUP);

  //RFID card
  SPI.begin();      // Initiate  SPI bus
 // mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");

}



void loop()
{


  // checkButton();
  //checkSwitchState();
  readCard();
  checkLightCoaster(A0,coasterValue1);
  checkLightCoaster(A1,coasterValue2);
  checkLightCoaster(A2,coasterValue3);
}

void readCard()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "BD 31 15 2B") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(3000);
  }
 
  else   
  {
    Serial.println(" Access denied");
    delay(3000);
  }
} 


void checkLightCoaster(int pin, float coasterValue)
{
  int analogValue = analogRead(pin);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
  Serial.println(lux);
}


void checkButtonSubmitDrink()
{

  buttonState = digitalRead(buttonPin);
  // NOT PRESSINGS
  if(buttonState == HIGH)
  {
    Serial.println("pressed");
  }
  //PRESSING
  else if (buttonState == LOW)
  {
    Serial.println("NOT pressed");
  }

}


void checkSwitchState()
{

  switchState = digitalRead(switchPin);
  // NOT PRESSINGS
  if(switchState == HIGH)
  {
    Serial.println("pressed");
  }
  //PRESSING
  else if (switchState == LOW)
  {
    Serial.println("NOT pressed");
  }
}
