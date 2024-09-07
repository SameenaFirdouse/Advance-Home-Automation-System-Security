#include <LiquidCrystal.h>
#include <Servo.h>
Servo myservo;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
unsigned char sdata=0,rx[40],r;
const int trigPin1 = A0;
const int echoPin1 = A1;
int us1,tl,mot=A3,buz=13,fir=A2,k='1';
void setup() 
{
  Serial.begin(9600);
  pinMode(mot,OUTPUT);
  pinMode(buz,OUTPUT);
  pinMode(fir,INPUT);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print("WATER LEVEL CTRL");
  lcd.setCursor(0, 1);
  lcd.print("   SYSTEM       ");
  
  //delay(2000);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("TANK LVL:       ");
  lcd.setCursor(0, 1);
  lcd.print("FIRE:NORMAL     ");
}
void loop() 
{
  //delay(300);
  u1();
  //Serial.println(us1);
  //u2();
  if(us1>1 && us1<31)
  tl=map(us1, 5,19,100,0);
  lcd.setCursor(9, 0);lcd.print(tl);lcd.print("% ");
  if(digitalRead(A2)==0 && k=='1')
  {
    Serial.println("ATD9502733981;");//delay(3000);
    //Serial.println("ATH");delay(2000);
    digitalWrite(buz,HIGH);
    k='0';
    lcd.setCursor(0, 1);
    lcd.print("FIRE:ACTIVATED  ");delay(300);
  }
  else if(digitalRead(A2)==1 && k=='0')
  {
    k='1';
    digitalWrite(buz,LOW);
    Serial.println("kk");
    lcd.setCursor(0, 1);
    lcd.print("FIRE:NORMAL     ");delay(300);
  }
  /*if(tl<10)
  {
    //digitalWrite(buz,HIGH);delay(50);
    //digitalWrite(buz,LOW);delay(10);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("TANK LVL:");lcd.print(tl);lcd.print("% ");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR:ON       ");
    //digitalWrite(mot,HIGH);
    delay(100);
  }
  else if(tl>99)
  {
    lcd.setCursor(0, 0);
    lcd.print("TANK LVL:");lcd.print(tl);lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR:OFF       ");
    digitalWrite(mot,LOW);
    delay(100);
  }  */
}
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
void u1()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration1, inches1, cm1;

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(trigPin1, OUTPUT);
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin1, INPUT);
  duration1 = pulseIn(echoPin1, HIGH);

  // convert the time into a distance
  inches1 = microsecondsToInches(duration1);
  cm1 = microsecondsToCentimeters(duration1);
  us1=cm1;
  /*Serial.print("U1:");
  Serial.print(cm1);
  Serial.print("cm");
  Serial.println("");*/
  
  delay(100);
}
