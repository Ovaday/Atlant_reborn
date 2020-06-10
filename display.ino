#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Thread.h>
Thread tempThread = Thread();
Thread dispThread = Thread();
Thread disperrorsThread = Thread();
#define ONE_WIRE_BUS 2  // датчики DS18B20 на 2 пин
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds(&oneWire);
DeviceAddress sensor2 = {0x28, 0x4E, 0x15, 0x43, 0x98, 0x22, 0x0, 0xE3};
DeviceAddress sensorout = {0x28, 0xDE, 0x0E, 0x43, 0x98, 0x01, 0x02, 0x76};
DeviceAddress sensorcold = {0x28, 0x33, 0x22, 0x43, 0x98, 0x22, 0x00, 0xE4};
DeviceAddress sensor1 = {0x28, 0x9D, 0x09, 0x43, 0x98, 0x01, 0x02, 0x0B};
DeviceAddress sensoreng = {0x28, 0xEA, 0x17, 0x43, 0x98, 0x23, 0x0, 0x24};

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
 int tempcold = 0;
 int temp1 = 0;
 int realt = 0;
 int needtemp = 4;
 int temp2 = 0;
 int tempout = 0;
 int tempeng = 0;
 int tempC;
 bool stengine = false;
 bool stcooler = false;
 bool ststarter = false;
 bool stheater = false;
 unsigned long timerdv =1;
 unsigned long timertick=0;
 unsigned long timing = 0;
 unsigned long heattimer = 1;
 unsigned long heat = 0;
 unsigned long cooldown = 0;
void setup() {
  tempThread.onRun(gettemp);     // назначаем потоку задачу
    tempThread.setInterval(200); // задаём интервал срабатывания, мсек
    dispThread.onRun(disp);     // назначаем потоку задачу
    dispThread.setInterval(600); // задаём интервал срабатывания, мсек
    disperrorsThread.onRun(disperrors);     // назначаем потоку задачу
    disperrorsThread.setInterval(2000); // задаём интервал срабатывания, мсек
  display.begin();
  display.setContrast(40);
  display.setTextSize(2);
 display.setRotation(2);
  ds.begin();
  ds.setResolution(sensorcold, 10);   
  ds.setResolution(sensor1, 10);
  ds.setResolution(sensor2, 10);
  ds.setResolution(sensorout, 10);
  ds.setResolution(sensoreng, 10);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(16, 0);
  pinMode(17, 0);
  pinMode(18, 0);
  pinMode(19, 0);
  engineon();


}
 
 
void loop(void) {
  if (tempThread.shouldRun())
        tempThread.run(); // запускаем поток
         if (disperrorsThread.shouldRun() && tempeng == -127)
        disperrorsThread.run(); // запускаем поток
        else if (disperrorsThread.shouldRun() && tempeng == 127)
        disperrorsThread.run();
         else if (disperrorsThread.shouldRun() && temp1 == -127)
          disperrorsThread.run();
          else if (disperrorsThread.shouldRun() && temp1 == 127)
          disperrorsThread.run();
          else{
          holod();
          }
           if (tempcold == 0 && temp1 == 0 && temp2 == 0 && tempout == 0 && millis() >= 5000)
           { 
            errorholod();
           }
  if (dispThread.shouldRun())
        dispThread.run();
        if ((millis()-timerdv) <= 12000000) timerdv = 0;
        if (millis() >= 4000000000) softReset();
}

void gettemp(){
  ds.requestTemperatures();
     tempout    = ds.getTempC(sensorout);
     tempeng    = ds.getTempC(sensoreng);
     temp1    = ds.getTempC(sensor1);
     temp2    = ds.getTempC(sensor2);
     tempcold    = ds.getTempC(sensorcold);
     realt = temp1;
     
   if(digitalRead(8)==1) needtemp = 1;
   else if(digitalRead(10)==1) needtemp = 3;
   else if(digitalRead(11)==1) needtemp = 5;
   else if(digitalRead(12)==1) needtemp = 7;
   else needtemp = 4;
}

void disp(){
  display.clearDisplay();
   display.clearDisplay();
  display.setTextSize(2);

  display.setTextSize(1);

  //морозильник
  if (tempcold == (-127) || tempcold == 127){
  display.setCursor(48,4);
  display.print("error");
  }
  else if (tempcold < 0)
{
  
  display.setCursor(42,4);
  display.print("-");
  display.setCursor(78,4);
  display.print("C");
  display.drawCircle(75, 4, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,0);
  display.print(tempcold*(-1));
}

  else
  {
  display.setCursor(42,4);
  display.print("+");
  display.setCursor(78,4);
  display.print("C");
  display.drawCircle(75, 4, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,0);
  display.print(tempcold);
  }
  
display.setTextSize(1);

  //среднее отделение
  
  if (temp1 == (-127) || temp1 == 127){
  display.setCursor(48,20);
  display.print("error");
  }
    else if (temp1 < 0)
{
  display.setCursor(42,20);
  display.println("-");
  display.setCursor(78,21);
  display.print("C");
  display.drawCircle(75, 21, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,17);
  display.println(temp1*(-1));
}
else
  {
  display.setCursor(42,20);
  display.println("+");
  display.setCursor(78,21);
  display.print("C");
  display.drawCircle(75, 21, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,17);
  display.println(temp1);
  }
  display.setTextSize(1);

  if (temp2 == (-127) || temp2 == 127){
  display.setCursor(48,37);
  display.print("error");
  }
    else if (temp2 < 0)
{
  //нижнее отделение
  display.setCursor(42,37);
  display.println("-");
    display.setCursor(78,39);
  display.print("C");
  display.drawCircle(75, 39, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,34);
  display.println(temp2*(-1));
  display.setTextSize(1);
}
  else
  {
    display.setCursor(42,37);
  display.println("+");
    display.setCursor(78,39);
  display.print("C");
  display.drawCircle(75, 39, 1, BLACK);
  display.setTextSize(2);
  display.setCursor(51,34);
  display.println(temp2);
  display.setTextSize(1);
  }

  if (tempout == (-127) || tempout == 127){
  display.setCursor(3,10);
  display.print("error");
  }
    else if (tempout < 0)
{
  //температура комнаты
  display.setCursor(8,10);
  display.print("-");
  display.setCursor(15,10);
  display.print(tempout*(-1));
}
else
{
  display.setCursor(8,10);
  display.print("+");
  display.setCursor(15,10);
  display.print(tempout);
}
if (tempeng == (-127) || tempeng == 127){
  display.setCursor(0,33);
  display.println("engine");
  display.setCursor(2,41);
  display.print("error");
  }
else{
  display.setCursor(0,41);
  display.print("E:");
  display.print(tempeng);
  display.setCursor(0,33);
  display.print("T:");display.print(needtemp); 
  display.setCursor(27,33);
  display.print("C");
  display.drawCircle(24, 34, 1, BLACK);
 
}

  display.setCursor(0,25);
  display.print("K:");display.print(heattimer/60000);
  //рисунки
   display.drawLine(1, 9, 4, 9, BLACK);
    display.drawLine(4,10 , 4, 21, BLACK);
   display.drawLine(5,21 , 32, 21, BLACK);
   display.drawLine(32,20 , 32,13 , BLACK);
   display.drawLine(31,13 , 30,13 , BLACK);
   display.drawLine(29,20 , 29,8 , BLACK);
   display.drawPixel(31, 17, BLACK);
    display.drawLine(29,9 , 32,9 , BLACK);
   display.drawLine(29,8 ,31 ,8 , BLACK);
   display.drawLine(29, 7, 27, 7, BLACK);
   display.drawLine(25, 6, 27, 6, BLACK);
   display.drawLine(23, 5, 25, 5, BLACK);
   display.drawLine(21, 4, 23, 4, BLACK);
   display.drawLine(19, 3, 21, 3, BLACK);
   display.drawLine(2,8,4 ,8, BLACK);
   display.drawLine(4, 7, 6, 7, BLACK);
   display.drawLine(6, 6, 8, 6, BLACK);
   display.drawLine(8, 5, 10, 5, BLACK);
   display.drawLine(10, 4, 12, 4, BLACK);
   display.drawLine(12, 3, 14, 3, BLACK);
   display.drawLine(14, 2, 19, 2, BLACK);
   display.drawLine(16, 1, 17, 1, BLACK);
   display.drawLine(29,9 , 32,9 , BLACK);
   display.drawLine(29,8 ,31 ,8 , BLACK);
   display.drawLine(29, 7, 27, 7, BLACK);
  display.drawLine(36, 15, 84, 15, BLACK);
  display.drawLine(36, 32, 84, 32, BLACK);
  display.drawLine(36, 0, 36, 48, BLACK);
  display.drawLine(34, 0, 34, 48, BLACK);
  display.drawLine(0, 24, 34, 24, BLACK);
  //рисунки
display.display();
}

void disperrors(){
  display.clearDisplay();
    display.setTextSize(2);
  display.setCursor(20,0);
  display.print("MANY");
  display.setCursor(8, 16); 
  display.print("ERRORS");
  display.setCursor(0, 32); 
  display.print("!");
  display.setCursor(76, 32); 
  display.print("!");
  display.setTextSize(1);
  display.setCursor(22,32);
  display.print("WORKING");
  display.setCursor(11,40);
  display.print("AUTOMATICLY");
  display.display();
}
void errorholod(){
  if (ststarter == true && (millis()-timertick) >= 3000) starteroff();
  switch (needtemp) {
    case 1:
     if (stengine == false && (millis()-timerdv) >= 1200000){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && tempcold >= (-14) && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && temp1 >= (3) && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineon();
  }
  
  if (stengine == true && (millis()-timertick) >= 1800000){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && tempcold <= (-18)  && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && temp1 <= (-1) && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineoff();
  }
   if (stengine == true && tempeng >= 85  && tempeng != 0 && tempeng != (-127) && tempeng != 127) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
      break;
    case 3:
      if (stengine == false && (millis()-timerdv) >= 1200000){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && tempcold >= (-11) && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && temp1 >= 5 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineon();
  }
  
  if (stengine == true && (millis()-timertick) >= 1200000){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && tempcold <= (-15)  && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && temp1 <= 1 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineoff();
  }
   if (stengine == true && tempeng >= 85  && tempeng != 0 && tempeng != (-127) && tempeng != 127) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
      break;
      case 5:
if (stengine == false && (millis()-timerdv) >= 1800000){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && tempcold >= (-8) && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && temp1 >= 7 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineon();
  }
  
  if (stengine == true && (millis()-timertick) >= 1200000){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && tempcold <= (-12)  && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && temp1 <= 3 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineoff();
  }
   if (stengine == true && tempeng >= 85  && tempeng != 0 && tempeng != (-127) && tempeng != 127) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
      break;
      case 7:
      if (stengine == false && (millis()-timerdv) >= 1800000){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && tempcold >= (-5) && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && temp1 >= 9 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineon();
  }
  
  if (stengine == true && (millis()-timertick) >= 600000){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && tempcold <= (-9)  && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && temp1 <= 5 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineoff();
  }
   if (stengine == true && tempeng >= 85  && tempeng != 0 && tempeng != (-127) && tempeng != 127) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
      break;
    default:
      if (stengine == false && (millis()-timerdv) >= 1200000){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && tempcold >= (-10) && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
  if (stengine == false && (millis()-timerdv) >= 600000 && temp1 >= 6 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineon();
  }
  
  if (stengine == true && (millis()-timertick) >= 1200000){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && tempcold <= (-14)  && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineoff();
  }
  if (stengine == true && (millis()-timertick) >= 600000 && temp1 <= 2 && temp1 != 0 && temp1 != (-127) && temp1 != 127){
    engineoff();
  }
   if (stengine == true && tempeng >= 85  && tempeng != 0 && tempeng != (-127) && tempeng != 127) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
  }
}

void holod(){
  if (stengine == false && tempeng <= 80 && (realt-needtemp)>= 2 && (millis()-timerdv) >= 1200000){
    engineon();
  }
  else if (stengine == false && tempeng <= 80 && tempcold >= (-2) && (millis()-timerdv) >= 1200000 && tempcold != 0 && tempcold != (-127) && tempcold != 127){
    engineon();
  }
   if (ststarter == true && (millis()-timertick) >= 3000) starteroff();
   if (stengine == true && (needtemp-realt) >= 2 && tempcold != 0 && tempcold != (-127) && tempcold != 127) engineoff();
   if (stengine == true && tempeng >= 85) engineoff();
   if (stengine == false && heattimer >= 43200000) heateron();
   if (stheater == true && (millis() - heat) >= 600000) heateroff();
  
}
void engineon(){
  stengine = true;
pinMode(19, 1);
  starteron();
  cooleron();
  timertick = millis();
}
void engineoff(){
  stengine = false;
  pinMode(19, 0);
  cooleroff();
  timerdv= millis();
  heattimer = heattimer + (timerdv - timertick);
}

void heateron(){
  stheater = true;
  cooleron();
  pinMode(17, 1);
  heattimer = 1;
  heat = millis();
}
void heateroff(){
  stheater = false;
  cooleroff();
  heattimer = 1;
  pinMode(17, 0);
}


void cooleron(){
  stcooler = true;
pinMode(16, 1);
}
void cooleroff(){
  stcooler = false;
pinMode(16, 0);
}
void starteron(){
  ststarter = true;
  pinMode(18, 1);
}
void starteroff(){
  ststarter = false;
  pinMode(18, 0);
}
void softReset(){
asm volatile ("  jmp 0");
}
/*void alarms(){
  else if ( TempZalC != -127    && TempZalC < 10    && KvitZalC==false )    {statusAlarm=1; AlarmTemp=1;}  // Сигнализация низкой температуры в зале +10
}*/
