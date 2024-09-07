#include<LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>
String exString;
SoftwareSerial gsm(9, 10);
Servo myservo;
int aval,sw1 = A0,sw2 = A3,sw3 = A2,rly = A5,buz = 13,sw_state,kkk=8,loc=A4,m2=A5;
unsigned char rdata,udata,page_id=0x00,schksum=0x0F,c,rdchk[20],uid[5],a[55],kdata,lcd_add=0x00,name[20];
unsigned int i,j,k,chk=0;
//RS=7,RW=Gnd,EN=6,D4=5,D5=4,D6=3,D7=2;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
unsigned char sdata_genimg[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x01,0x00,0x05};
unsigned char sdata_img2char1[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x02,0x01,0x00,0x08};
unsigned char sdata_img2char2[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x02,0x02,0x00,0x09};
unsigned char sdata_gentemp[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x05,0x00,0x09};
unsigned char sdata_store[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x06,0x06,0x02,0x00,0x00,0x00,0x0f};
unsigned char sdata_search[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x1B,0x01,0x00,0x00,0x03,0xE9,0x01,0x11};
unsigned char sdata_empty[] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x0D,0x00,0x11};
void setup()
{
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT);
  pinMode(rly,OUTPUT);
  pinMode(loc,OUTPUT);
  pinMode(m2,OUTPUT);
   pinMode(kkk,OUTPUT);
  pinMode(buz,OUTPUT);  
  myservo.attach(11);
  lcd.begin(16, 2);
  Serial.begin(57600);gsm.begin(9600);
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print("FINGERPRINT BASE");
  lcd.setCursor(0,1);
  lcd.print("DOOR LOCK SYSTEM");
  delay(2000);
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print(" SW1   SW2   SW3");
  lcd.setCursor(0,1);
  lcd.print(" ENR   ERS   SER");
  delay(200);
  myservo.write(0);
  //delay(1000);
  //myservo.write(180);
  //delay(1000);
  //myservo.write(0);
  RecieveMessage();
  delay(1000);
}
 void RecieveMessage()
{
  gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  gsm.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
}
void loop()
{
  if (gsm.available()>0)
{ 
  exString=gsm.readString();
  //Serial.println(exString);
  //Serial.println(exString.substring(12,22));
  //Serial.println("--------");
  //Serial.println(exString.substring(51,54));  
  //Serial.println("--------");
  if(exString.substring(51,54)=="OPN")
  {
    
    //Serial.println("TURN 1--------ok");
  //gps('3');
  myservo.write(180);
  delay(2000);myservo.write(0);
  
  }
}
  scan();
}
void scan()
{
  sw_state = digitalRead(sw1);  // check if the pushbutton is pressed.
  if (sw_state == LOW) 
  { 
    lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print("PLACE FINGER ON ");
    lcd.setCursor(0,1);
    lcd.print("SENSOR TO ENROLL");
    delay(500);
    enroll();
  } 
  delay(10);
  //*********************************************************
  sw_state = digitalRead(sw2);  // check if the pushbutton is pressed.
  if (sw_state == LOW) 
  { 
    lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print("YOU SELECTED TO ");
    lcd.setCursor(0,1);
    lcd.print(" ERASE DATABASE ");
    delay(500);
    erase();
  } 
  delay(10);
  //*********************************************************
  sw_state = digitalRead(sw3);  // check if the pushbutton is pressed.
  if (sw_state == LOW) 
  { 
    lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print("PLACE FINGER ON ");
    lcd.setCursor(0,1);
    lcd.print("SENSOR TO SEARCH");
    delay(500);
    search();
  } 
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print(" SW1   SW2   SW3");
  lcd.setCursor(0,1);
  lcd.print(" ENR   ERS   SER");
  delay(10);
}
//*********************************************************
void enroll()
{
  genimg();
  delay(10);
  img2char1();
  delay(10);
  genimg();
  delay(10);
  img2char2();
  delay(10);
  gentemp();
  delay(10);
  storetemp();
  delay(300);
}
//*********************************************************
void erase()
{
  lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
  lcd.print(" ERASING FP DATA");
  lcd.setCursor(0,1);
  lcd.print(" IN THE SENSOR  ");
  delay(50);
  chk=0;
  for(c=0;c<12;c++)
  {
    Serial.write(sdata_empty[c]);
  }
  for(i=0;i<12;i++)
  {
    rxd();
    rdchk[i]=rdata;
  }
  sucess_rep();
  if (chk==1)
  {
    lcd.setCursor(0,0);
    lcd.print("DATABASE ERASED ");
    lcd.setCursor(0,1);
    lcd.print("  SUCESSFULLY.  ");
  }
  else if(chk==0)
  {
    lcd.setCursor(0,0);
    lcd.print("FAILED TO CLEAR ");
    lcd.setCursor(0,1);
    lcd.print("USER FP DATABASE");
  }
}
//*********************************************************
void search()
{
  genimg();
  img2char1();
  fp_search();
}
//**************************************************************
void rxd()
{
  while(Serial.available()==0);
  rdata = Serial.read();
}
void txd(unsigned char tdata)
{
  Serial.write(tdata);
}
void hex2asc()
{
  udata=rdchk[11];
  for(i=0;i<3;i++)
  {
    uid[i] = udata%10;
    uid[i] = uid[i]|0x30;
    udata = udata/10;
  }
}
//***$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$********************
void genimg()
{
  chk=0;
  while(chk==0)
  {
    for(c=0;c<12;c++)
    {
      Serial.write(sdata_genimg[c]);
    }
    for(i=0;i<12;i++)
    {
      rxd();
      rdchk[i]=rdata;
    }
    sucess_rep();
    if (chk==1)
    {
      lcd.setCursor(0,0);
      lcd.print(" FINGER DETECTED");
      lcd.setCursor(0,1);
      lcd.print("  ON THE SENSOR ");
    }
    else if(chk==0)
    {
      lcd.setCursor(0,0);
      lcd.print("  NO FINGER ON  ");
      lcd.setCursor(0,1);
      lcd.print("   THE SENSOR   ");
      //digitalWrite(rly1,1);
      //digitalWrite(buz,1);
    }
  }
  delay(10);
}
//*************************** img2char *******************************
void img2char1()
{
  chk=0;
  for(c=0;c<13;c++)
  {
    Serial.write(sdata_img2char1[c]);
  }
  for(i=0;i<12;i++)
  {
    rxd();
    rdchk[i]=rdata;
  }
  sucess_rep();
  if (chk==1)
  {

  }
  else if(chk==0)
  {
    lcd.setCursor(0,0);
    lcd.print("IMG1 GENERATION ");
    lcd.setCursor(0,1);
    lcd.print("FAILED,TRY AGAIN");
  }
  delay(10);
}
//********************************************************************
void img2char2()
{
  chk=0;
  for(c=0;c<13;c++)
  {
    Serial.write(sdata_img2char2[c]);
  }
  for(i=0;i<12;i++)
  {
    rxd();
    rdchk[i]=rdata;
  }
  sucess_rep();
  if (chk==1)
  {

  }
  else if(chk==0)
  {
    lcd.setCursor(0,0);
    lcd.print("IMG2 GENERATION ");
    lcd.setCursor(0,1);
    lcd.print("FAILED,TRY AGAIN");
  }
  delay(10);
}
//********************************************************************
void gentemp()
{
  chk=0;
  for(c=0;c<12;c++)
  {
    Serial.write(sdata_gentemp[c]);
  }
  for(i=0;i<12;i++)
  {
    rxd();
    rdchk[i]=rdata;
  }
  sucess_rep();
  if (chk==1)
  {

  }
  else if(chk==0)
  {
    lcd.setCursor(0,0);
    lcd.print("TEMP GENERATION ");
    lcd.setCursor(0,1);
    lcd.print("FAILED,TRY AGAIN");
  }
  delay(10);
}
//********************************************************************
void storetemp()
{
  sdata_store[12]=page_id;
  sdata_store[14]=schksum;
  chk=0;
  for(c=0;c<15;c++)
  {
    Serial.write(sdata_store[c]);
  }
  for(i=0;i<12;i++)
  {
    rxd();
    rdchk[i]=rdata;
  }
  sucess_rep();
  if (chk==1)
  {
    lcd.setCursor(0,0);
    lcd.print("TEMPLATE STORED ");
    lcd.setCursor(0,1);
    lcd.print("   SUCESSFULLY  ");
    delay(200);
    rdchk[11]=page_id;
    hex2asc();
    lcd.setCursor(0,0);
    lcd.print(" YOUR TEMPLATE  ");
    lcd.setCursor(0,1);
    lcd.print("ID NO:          ");
    lcd.setCursor(7,1);
    lcd.write(uid[2]);
    lcd.write(uid[1]);
    lcd.write(uid[0]);
    delay(100);
  }
  else if(chk==0)
  {
    lcd.setCursor(0,0);
    lcd.print("TEMPLATE STORING");
    lcd.setCursor(0,1);
    lcd.print("FAILED,TRY AGAIN");
  }
  delay(10);
  page_id++;
  schksum++;
}
//*********************************************************
void sucess_rep()		// Will check the reply from R305 and return status in 'chk'.
{
  if(rdchk[9]==0x00 && rdchk[11]==0x0a)
  {
    chk=1;
  }
  else 
  {
    chk=0;
  }
}
//*************************** SEARCH USER *****************************************
void fp_search()
{
  chk=0;
  for(c=0;c<17;c++)
    {
    Serial.write(sdata_search[c]);
    }
  for(i=0;i<16;i++)
    {
    rxd();
    rdchk[i]=rdata;
    }
  if(rdchk[9]==0x00)
    {
    chk=1;
    hex2asc();
    lcd.setCursor(0,0);
    lcd.print("   USER VALID.! ");
    lcd.setCursor(0,1);
    lcd.print("USER-ID:        ");
    lcd.setCursor(9,1);
    lcd.write(uid[2]);
    lcd.write(uid[1]);
    lcd.write(uid[0]);
    myservo.write(90);
    digitalWrite(loc,HIGH);
    //digitalWrite(m1,HIGH);digitalWrite(m2,LOW);
    delay(2000);
    //digitalWrite(m1,LOW);digitalWrite(m2,LOW);
    delay(1000);
    //digitalWrite(m1,LOW);digitalWrite(m2,HIGH);
    delay(2000);
    //digitalWrite(m1,LOW);digitalWrite(m2,LOW);
    //while(digitalRead(sw3)==1);
    myservo.write(0);
    digitalWrite(loc,LOW);
    delay(200);
    lcd.begin(16, 2);
    lcd.setCursor(0,0);  //lcd.setCursor(column,line) starts with 0
    lcd.print(" SW1   SW2   SW3");
    lcd.setCursor(0,1);
    lcd.print(" ENR   ERS   SER");
    }
else if(rdchk[9]==0x09)
    {
    digitalWrite(buz,1);
    chk=0;
    lcd.setCursor(0,0);
    lcd.print("  USER INVALID  ");
    lcd.setCursor(0,1);
    lcd.print("NO FINGER PRINT.");
    digitalWrite(buz,HIGH);
    SendMessage();
    digitalWrite(buz,LOW);
    digitalWrite(kkk,LOW);
    delay(2000);
    //digitalWrite(rly1,1);
    }
  delay(10);
}
void SendMessage()
{
  gsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(2000);  // Delay of 1000 milli seconds or 1 second
  gsm.println("AT+CMGS=\"9492932187\"\r"); // Replace x with mobile number
  delay(2000);
  gsm.println("UNAUTHORISED PERSON ENTERED PLEASE TAKE ALERT..!");// The SMS text you want to send
  delay(2000);
   gsm.println((char)26);// ASCII code of CTRL+Z
  delay(2000);
}
