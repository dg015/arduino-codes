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


// colour detection
#define s0 3
#define s1 4
#define s2 5
#define s3 6
#define out 7
int   Red=0, Blue=0, Green=0;  //RGB values 

String currentColour;

void setup()
{
  Serial.begin(115200);

  //Send button
  pinMode(buttonPin, INPUT_PULLUP);

  // beer tap
  pinMode(switchPin, INPUT_PULLUP);

  //colour detection pin setup
  pinMode(s0,OUTPUT);    
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);

  digitalWrite(s0,HIGH);

  //RFID card
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
}



void loop()
{
  //checkButton();
  //checkSwitchState();
  //readCard();

  //coaster
  //checkLightCoaster(A0,coasterValue1);
  //checkLightCoaster(A1,coasterValue2);
  //checkLightCoaster(A2,coasterValue3);
  getColour();

}

void getColour()
{

    GetColors();                                     //Execute the GetColors function   to get the value of each RGB color
                                                   //Depending   of the RGB values given by the sensor we can define the color and displays it on   the monitor

  if (Red <=15 && Green <=15 && Blue <=15)         //If the values   are low it's likely the white color (all the colors are present)
      {
      currentColour = "White";
      Serial.println("White");                     
      }
      
  else if (Red<Blue && Red<=Green && Red<23)      //if   Red value is the lowest one and smaller thant 23 it's likely Red
      {
        currentColour = "Red";
        //Serial.println("Red");
      }

   else if (Blue<Green && Blue<Red && Blue<20)    //Same thing for Blue
      {
        currentColour = "Blue";
        //Serial.println("Blue");
      }

   else if (Green<Red && Green-Blue<= 8)           //Green it was a little tricky,   you can do it using the same method as above (the lowest), but here I used a reflective   object
      {
        currentColour = "Green";
        //Serial.println("Green");                    //which means the   blue value is very low too, so I decided to check the difference between green and   blue and see if it's acceptable
      }
  else
     {
      currentColour = "Unknown";
      //Serial.println("Unknown");                   //if the color is not recognized, you can add as many as you want
     }

   delay(2000);                                   //2s delay you can modify if you   want

}



void GetColors()  
{    
  digitalWrite(s2,   LOW);                                           //S2/S3 levels define which set   of photodiodes we are using LOW/LOW is for RED LOW/HIGH is for Blue and HIGH/HIGH   is for green 
  digitalWrite(s3, LOW);                                           
   Red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);       //here we wait   until "out" go LOW, we start measuring the duration and stops when "out" is   HIGH again, if you have trouble with this expression check the bottom of the code
   delay(20);  
  digitalWrite(s3, HIGH);                                         //Here   we select the other color (set of photodiodes) and measure the other colors value   using the same techinque
  Blue = pulseIn(out, digitalRead(out) == HIGH ? LOW   : HIGH);
  delay(20);  
  digitalWrite(s2, HIGH);  
  Green = pulseIn(out,   digitalRead(out) == HIGH ? LOW : HIGH);
  delay(20);  
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
  //data order COASTERS -> BUTTON -> SWITCH -> colour
  Serial.print("DATA");
  Serial.print(",");

  //coaster data
  Serial.print(coasterValue1);
  Serial.print(",");
  Serial.print(coasterValue2);
  Serial.print(",");
  Serial.print(coasterValue3);
  Serial.print(",");

  //button data
  Serial.print(buttonState);
  Serial.print(",");

  // Switch data
  Serial.print(switchState);
  Serial.print(",");
  Serial.print(currentColour);
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
