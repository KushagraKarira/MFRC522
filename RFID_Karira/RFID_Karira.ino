/******************************************
  Created by  Rudy Schlaf for www.makecourse.com
  Updated     1/2020 by Kushagra Karira
*******************************************/
//** I Kushagra Karira has relesed this modified version of MFRC522 tutorial on Github (github.com/kushagrakarira/iota) as the source file for team iota, accessible after August 2020**//
/*
  This sketch uses the MFRC522 Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
  The library file MFRC522.h has a wealth of useful info. Please read it.
  The functions are documented in MFRC522.cpp.

  Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
  Created by Miguel Balboa (circuitito.com), Jan, 2012.
  Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013
  Updated by Kushagra Karira ( kushagrakarira.github.io )

  This library has been released into the public domain.

*  * Typical pin layout used:
  -----------------------------------------------------------------------------------------
  MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
  Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
  Signal      Pin          Pin           Pin       Pin        Pin              Pin
  -----------------------------------------------------------------------------------------
  RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
  SPI SS      SDA(SS)      10            53        D10        10               10
  SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
  SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
  SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

*/

#include <SPI.h>//include the SPI bus library
#include <MFRC522Extended.h>//include the RFID reader library

#define SS_PIN 10  // Refer above layout for details
#define RST_PIN 9  // Refer above layout for details
MFRC522 mfrc522(SS_PIN, RST_PIN);        // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key;//create a MIFARE_Key struct named 'key', which will hold the card information


void setup() {
  Serial.begin(9600);        // Initialize serial communications with the PC
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card (in case you wonder what PCD means: proximity coupling device)
  //Serial.println("Scan a MIFARE Classic card");

  // Prepare the security key for the read and write functions - all six key bytes are set to 0xFF at chip delivery from the factory.
  // Since the cards in the kit are new and the keys were never defined, they are 0xFF
  // if we had a card that was programmed by someone else, we would need to know the key to be able to access it. This key would then need to be stored in 'key' instead.

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;//keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
  }

}

//int block = 2; //this is the block number we will write into and then read. Do not write into 'sector trailer' block, since this can make the block unusable.

//an array with 16 bytes to be written into one of the 64 card blocks is defined
//byte blockcontent[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//all zeros. This can be used to delete a block.

byte readbackblock[18];//This array is used for reading out a block. The MIFARE_Read method requires a buffer that is at least 18 bytes to hold the 16 bytes of a block.


void loop()
{

  /*****************************************establishing contact with a tag/card**********************************************************************/

  if ( ! mfrc522.PICC_IsNewCardPresent()) {//if PICC_IsNewCardPresent returns 1, a new card has been found and we continue
    return;//if it did not find a new card is returns a '0' and we return to the start of the loop
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {//if PICC_ReadCardSerial returns 1, the "uid" struct (see MFRC522.h) contains the ID of the read card.
    return;//if it returns a '0' something went wrong and we return to the start of the loop
  }

  /*****************************************writing and reading a block on the card**********************************************************************/
  writeBlock( 8,"1234 1234 1234" );
  writeBlock( 9,"Kushagra Karira" );
  writeBlock( 10,"19, O+, Normal" );
  
  for (int i = 8; i < 11; i++)
  {
    if ( (i + 1) % 4 != 0 )
    {
      readBlock(i, readbackblock);//read the block back
      for (int j = 0 ; j < 16 ; j++) //print the block contents
      {
        Serial.write (readbackblock[j]);//Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
      }
      Serial.println("");
    }
  }

}
