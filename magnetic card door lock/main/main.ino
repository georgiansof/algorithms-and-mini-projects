#include <LiquidCrystal.h>
#include <SPI.h> 
#include <RFID.h> 

LiquidCrystal lcd(2,3,4,5,6,7);
RFID rfid(10, 9);       
unsigned char status; 
unsigned char str[MAX_LEN]; 

String accessGranted [1] = {"9121235113"};  
int accessGrantedSize = 1;                                

void InitLCD() {
  lcd.clear();
  lcd.print(" Scan magnetic");
  lcd.setCursor(0, 1);
  lcd.print("     card -> ");
}

void setup() 
{ 
  lcd.begin(16,2);
  InitLCD();

  Serial.begin(9600);     
  SPI.begin();            
  rfid.init();            
  Serial.println("Place card/tag near reader...");
} 

void loop() 
{ 
  if (rfid.findCard(PICC_REQIDL, str) == MI_OK)  
  { 
    Serial.println("Card found"); 
    String temp = "";                           
    if (rfid.anticoll(str) == MI_OK)              
    { 
      Serial.print("The card's ID number is : "); 
      for (int i = 0; i < 4; i++)                
      { 
        temp = temp + (0x0F & (str[i] >> 4)); 
        temp = temp + (0x0F & str[i]); 
      } 
      Serial.println (temp);
      checkAccess (temp);     
    } 
    rfid.selectTag(str); 
  }
  rfid.halt();
}

void checkAccess (String temp)    
{
  boolean granted = false;
  for (int i=0; i <= (accessGrantedSize-1); i++)    
  {
    if(accessGranted[i] == temp)            
    {
      Serial.println ("Access Granted");
      granted = true;
      lcd.clear();
      lcd.print("Access Granted !");
      delay(5000);
      InitLCD();
    }
  }
  if (granted == false)     //If the tag is not found
  {
    Serial.println ("Access Denied");
    lcd.clear();
    lcd.print("Access Denied !");
    delay(5000);
    InitLCD();
  }
}
