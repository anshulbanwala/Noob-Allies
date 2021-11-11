#include<SPI.h>
#include <MFRC522.h>
#define SS_PIN 10  
#define RST_PIN 7  
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;          
String Name;
String ID;

int blockNum1 = 2;  //location in the tag where we want to save our data
int blockNum2 = 3;

byte bufferLen = 18;
byte nameBlockData[18];
byte idBlockData[18];
MFRC522::StatusCode status;

void setup() {
  /* Initialize serial communications with the PC */
  Serial.begin(9600);
  /* Initialize SPI bus */
  SPI.begin();
  /* Initialize MFRC522 Module */
  mfrc522.PCD_Init();
  Serial.println("LABEL,Date,Time,Name, Army ID");
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
  Serial.print("\n");
  Serial.println("Reading from Data Block...");
  ReadDataFromBlock(blockNum1, nameBlockData);

  for(int j=0;j<16;j++){
      Name=Serial.write(nameBlockData[j]);
    }
    
  ReadDataFromBlock(blockNum2, idBlockData);

  for(int j=0;j<16;j++){
      ID=Serial.write(idBlockData[j]);
    }
  

  Serial.println( (String) "DATA,DATE,TIME," + Name + ID );
}

void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{
  /* Authenticating the desired data block for Read access using Key A */
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));

  if (status != MFRC522::STATUS_OK)
  {
     Serial.print("Authentication failed for Read: ");
     Serial.println(mfrc522.GetStatusCodeName(status));
     return;
  }
  else
  {
    Serial.println("Authentication success");
  }

  /* Reading data from the Block */
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print("Reading failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else
  {
    Serial.println("Data was read successfully"); 
    
}
  
}
