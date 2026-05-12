// C++ code
//

//card reader
#include <SPI.h>
#include <MFRC522.h>

//accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 

Adafruit_MPU6050 mpu;


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


void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);
  SPI.begin();      // Initiate  SPI bus
 // mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  /*
  mpu.begin();
    while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(1000);
  }
  Serial.println("MPU6050 ready!");
  */
}

sensors_event_t event;


void loop()
{
 // checkButton();
 //checkSwitchState();
 readCard();

  /*
      mpu.getAccelerometerSensor()->getEvent(&event);

  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(event.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(event.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(event.acceleration.z);
  Serial.println(" m/s^2");
  delay(500);
*/

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
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }
} 


void accelerometer()
{
    mpu.getAccelerometerSensor()->getEvent(&event);

  Serial.print("[");
  Serial.print(millis());
  Serial.print("] X: ");
  Serial.print(event.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(event.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(event.acceleration.z);
  Serial.println(" m/s^2");
  delay(500);


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
