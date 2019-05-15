//////////////////////////////////////////////
// Используемые библиотерки и их выводы //////
//////////////////////////////////////////////
#include <OneWire.h>  ////////////////////////
#include <DallasTemperature.h> ///////////////
#include <LiquidCrystal.h>  //////////////////
OneWire oneWire(13); /////////////////////////
DallasTemperature sensors(&oneWire);//////////    
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//////////
#define rele1 12 /////////////////////////////
#define rele2 11 /////////////////////////////
#define rele3 3  /////////////////////////////
#define rele4 2  /////////////////////////////
int r1; //////////////////////////////////////
int r2; //////////////////////////////////////
int r3; //////////////////////////////////////
int r4; //////////////////////////////////////
const int BUTTON_NONE = 0; ///////////////////
const int BUTTON_RIGHT = 1; //////////////////
const int BUTTON_UP = 2; /////////////////////
const int BUTTON_DOWN = 3; ///////////////////
const int BUTTON_LEFT = 4; ///////////////////
const int BUTTON_SELECT = 5; /////////////////
const int TempLvL0 = 0; //////////////////////
const int TempLvL1 = 1; //////////////////////
const int TempLvL2 = 2; //////////////////////
const int TempLvL3 = 3; //////////////////////
const int TempLvL4 = 4; //////////////////////
int TempCritical = 36; ///////////////////////
int TempHigh = 31; ///////////////////////////
int TempMid = 28; ////////////////////////////
int TempLow = 26; ////////////////////////////
int TH; //////////////////////////////////////
int TM; //////////////////////////////////////
int TL; //////////////////////////////////////
unsigned long lasttime; // for millis timer //
float Temp;///////////////////////////////////
int button; //////////////////////////////////
int TempRound; ///////////////////////////////
int TempLvl;  ////////////////////////////////
int getPressedButton() { /////////////////////
  int buttonValue = analogRead(0); ///////////
  if (buttonValue < 100) { ///////////////////
    return BUTTON_RIGHT; /////////////////////
  } //////////////////////////////////////////
  else if (buttonValue < 200) { //////////////
    return BUTTON_UP; ////////////////////////
  } //////////////////////////////////////////
  else if (buttonValue < 400) { //////////////
    return BUTTON_DOWN; //////////////////////
  } //////////////////////////////////////////
  else if (buttonValue < 600) { //////////////
    return BUTTON_LEFT; //////////////////////
  } //////////////////////////////////////////
  else if (buttonValue < 800) { //////////////
    return BUTTON_SELECT; ////////////////////
  } //////////////////////////////////////////
  return BUTTON_NONE; ////////////////////////
} ////////////////////////////////////////////
int getTempLvl() { ///////////////////////////
  int Temp = sensors.getTempCByIndex(0);//////
  if (round(Temp) >= TempCritical) {//////////
    return TempLvL4;//////////////////////////
  }///////////////////////////////////////////
  else if (round(Temp) >= TempHigh) {/////////
    return TempLvL3;//////////////////////////
  }///////////////////////////////////////////
  else if (round(Temp) >= TempMid) {//////////
    return TempLvL2;//////////////////////////
  }
  else if (round(Temp) >= TempLow) {
    return TempLvL1;
  }
  else if (round(Temp) <= TempLow) {
    return TempLvL0;
  }
}
void setup() { ///////////////////////////////
  Serial.begin(9600);
  sensors.begin();
  lcd.begin(16, 2);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);
  digitalWrite(rele3, HIGH);
  digitalWrite(rele4, HIGH);
  TempRound = round(Temp);
}
void loop() { ////////////////////////////////
  button = getPressedButton();
  TempLvl = getTempLvl();
  sensors.requestTemperatures(); // считываем температуру с датчиков
  Temp = sensors.getTempCByIndex(0);  // Получаем температуру с датчика 1
  TempRound = round(Temp);
  TH = round(TempHigh);
  TM = round(TempMid);
  TL = round(TempLow);
  if (( Temp > 20 ) && ( button == 0 )){ // Отображение температуры, если не нажата ниодна кнопка
  lcd.setCursor(0, 0);
  lcd.print("Temp Now: ");
  lcd.print(Temp);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(TempLow);
  lcd.print(",M:");
  lcd.print(TempMid);
  lcd.print(",H:");
  lcd.print(TempHigh);
  }
  switch (button) {
    case BUTTON_RIGHT: // при нажатии кнопки выводим следующий текст
      TempHigh++;
      lcd.setCursor(0, 1);
      lcd.print("L:");
      lcd.print(TempLow);
      lcd.print(",M:");
      lcd.print(TempMid);
      lcd.print(",H:");
      lcd.print(TempHigh);
      break;
    case BUTTON_LEFT:
      TempHigh--;
      lcd.setCursor(0, 1);
      lcd.print("L:");
      lcd.print(TempLow);
      lcd.print(",M:");
      lcd.print(TempMid);
      lcd.print(",H:");
      lcd.print(TempHigh);
      break;
    case BUTTON_UP:
      TempHigh++;
      TempMid++;
      TempLow++;
      lcd.setCursor(0, 1);
      lcd.print("L:");
      lcd.print(TempLow);
      lcd.print(",M:");
      lcd.print(TempMid);
      lcd.print(",H:");
      lcd.print(TempHigh);
      break;
    case BUTTON_DOWN:
      TempHigh--;
      TempMid--;
      TempLow--;
      lcd.setCursor(0, 1);
      lcd.print("L:");
      lcd.print(TempLow);
      lcd.print(",M:");
      lcd.print(TempMid);
      lcd.print(",H:");
      lcd.print(TempHigh); 
      break;
    case BUTTON_SELECT:
      lcd.setCursor(0, 0);
      lcd.print("                 ");
      lcd.setCursor(0, 0);
      lcd.print("Set Max Temp");
      break;
  }
  switch (TempLvl){
    case TempLvL0: // t ниже 26С. В работе однофазные
      digitalWrite(rele1, HIGH);
      digitalWrite(rele2, LOW);
      digitalWrite(rele3, LOW);
      digitalWrite(rele4, LOW);
      break;
   case TempLvL1: // t выше 26С. Работают трехфазные к соседям
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, HIGH);
      digitalWrite(rele3, LOW);
      digitalWrite(rele4, LOW);
      break;
   case TempLvL2: // t выше 28С. Работает крыша малый
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, LOW);
      digitalWrite(rele3, HIGH);
      digitalWrite(rele4, LOW);
      break;
   case TempLvL3: // t выше 30С. Работает малый крыша и трехфазники соседи.
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, HIGH);
      digitalWrite(rele3, HIGH);
      digitalWrite(rele4, LOW);
      break;
   case TempLvL4: // t выше 36С. Это хуево. Работают два на крыше
      digitalWrite(rele1, LOW);
      digitalWrite(rele2, LOW);
      digitalWrite(rele3, HIGH);
      digitalWrite(rele4, HIGH);
      break;
  }
if (millis() - lasttime > 10000) {
    lasttime = millis();
    r1 = digitalRead(rele1);
    r2 = digitalRead(rele2);
    r3 = digitalRead(rele3);
    r4 = (digitalRead(rele4));
    Serial.print("Temp Now");
    Serial.print(": ");
    Serial.print(sensors.getTempCByIndex(0)); // отправляем температуру
    Serial.println("C");
    Serial.print("Temp Level ");
    Serial.println(TempLvl);
    Serial.println("Working propellers: ");
    Serial.print ("#1 - ");
    Serial.println (r1);
    Serial.print ("#1 - ");
    Serial.println (r2);
    Serial.print ("#3 - ");
    Serial.println (r3);
    Serial.print ("#4 - ");
    Serial.println (r4);
    }   
  }
