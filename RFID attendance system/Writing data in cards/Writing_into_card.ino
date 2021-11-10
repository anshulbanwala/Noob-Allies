#include<SPI.h>
#include <MFRC522.h>
#define SS_PIN 10  
#define RST_PIN 7  
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;          
int blockNum = 2;  //location in the tag where we want to save our data
byte blockData [16]; //array to store the person's name
String person_name;//variable to take the person's name
byte bufferLen = 18;
byte readBlockData[18];
MFRC522::StatusCode status;

void setup() {
  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  Serial.println("Scan a MIFARE 1K Tag to write data...");
}

void loop() {
  /* Look for new cards */
  /* Reset the loop if no new card is present on RC522 Reader */
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  
  /* Select one of the cards */
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("\n");
  Serial.println("**Card Detected**");
  /* Print UID of the Card */
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.print("\n");
  Serial.print("Enter the name of the person: ");
  while(Serial.available()==0){
    //wait for the user input
  }
  person_name=Serial.readString();
  person_name.getBytes(blockData,16);
  Serial.print("\n");
  Serial.println("Writing to Data to the card...");
  WriteDataToBlock(blockNum, blockData);
}

void WriteDataToBlock(int blockNum, byte blockData[]) 
{
  /* Authenticating the desired data block for write access using Key A */
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Authentication failed for Write: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Authentication success");
  }


  
  /* Write data to the block */
  status = mfrc522.MIFARE_Write(blockNum, blockData, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Writing failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was written successfully");
  }
  
}
