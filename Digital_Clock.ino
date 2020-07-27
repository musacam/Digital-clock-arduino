#include <LiquidCrystal.h>

LiquidCrystal lcd(12,11,4,5,6,7);

const int b1=2;		//Button1
const int b3=3;		//Button2
const int b2=8;		//Button3
const int b4=9;		//Button4
const int alarm=10;
int temp;
int val4;	//digitalRead(b4)
int hour = 16;
int min=28;
int sec=0;
int alarmHour = 16;
int alarmMin = 30;
int flag1=0;
int flag2=0;
int flag3=0;

void setup()
  
{
  
  cli(); 
  Serial.begin(9600);
  
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  pinMode(b3,INPUT_PULLUP);
  pinMode(b4,INPUT_PULLUP);
  pinMode(alarm,OUTPUT);
  pinMode(A0,INPUT);
 
  TCNT1= 0; 
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A=15624; 
  TCCR1A |=(1 << WGM12);
  TCCR1B |=(1 << CS11)| (1 << CS10) ; //256
  TIMSK1 |=(1 << OCIE1A); 
  
  lcd.begin(16,2);
  
   attachInterrupt(digitalPinToInterrupt(b3),TempFlag, FALLING);
   attachInterrupt(digitalPinToInterrupt(b1),TimeFlag, FALLING);
   attachInterrupt(digitalPinToInterrupt(b2),setTime, FALLING);
   sei();
  
  
}

void loop(){
    lcd.setCursor(2,1);
    lcd.print(":");
    AlarmTime(alarmHour, alarmMin);
    temp=analogRead(A0);
    val4=digitalRead(b4);
    ClockTime(hour,min);
    DisplayTemp(temp);
  	setClock();
    alarmRing();
    snooze();
}

 ISR(TIMER1_COMPA_vect)
 
{
  sec++;
  if(sec>=60){
   min++;
   sec=0;   
  }
  if(min>=60){
   hour++;
   min=0;
  }
  if(hour>=24){
   hour=0; 
  }
   
}

void AlarmTime(int alarmHour, int alarmMin){

  lcd.setCursor(0,0);
    lcd.print("Alarm");
    lcd.setCursor(5,0);
    lcd.print(":");
    
    if(alarmHour<10){
      lcd.setCursor(6,0);
      lcd.print("0");
      lcd.setCursor(7,0);
      lcd.print(alarmHour);
    }
    
    else{
         lcd.setCursor(6,0);  
         lcd.print(alarmHour);
        }
        
    lcd.setCursor(8,0);
    lcd.print(":");
    
    if(alarmMin<10){
      lcd.setCursor(9,0);
      lcd.print("0");
      lcd.setCursor(10,0);
      lcd.print(alarmMin);
    }
    
    else{
         lcd.setCursor(9,0);  
         lcd.print(alarmMin);
        }
}

void ClockTime(int hour,int min){
     if(flag1)
    {
          
          if(hour>12 && hour < 22)
          {
            lcd.setCursor(0,1);
            lcd.print("0");
            lcd.setCursor(1,1);
            lcd.print(hour-12);
            lcd.setCursor(5,1);
            lcd.print("PM");
          }
          else if (hour<12)
          {
            if(hour<10){
            	lcd.setCursor(0,1);
              	lcd.print("0");
              	lcd.setCursor(1,1);
              	lcd.print(hour);
            }
            else{
            lcd.print(hour);
            }
            lcd.setCursor(5,1);
            lcd.print("AM");
          }
          else if  (hour==12) 
          {
            lcd.print(hour);
            lcd.setCursor(5,1);
            lcd.print("PM");
          }
          
    }
//24H Format
    else
    {
        lcd.setCursor(0,1);
      	if(hour==0){ lcd.print("00"); }
        else if(hour<10){
            lcd.setCursor(0,1);
            lcd.print("0");
            lcd.setCursor(1,1);
            lcd.print(hour);
        }
        else{
            lcd.print(hour);
        }
        lcd.setCursor(5,1);
        lcd.print("  ");
    }
   
     lcd.setCursor(3,1);
     if(min==0){
      lcd.print("00");
     }
     else if(min>0 && min < 10){
        lcd.print("0");
        lcd.print(min);
     }
     else{
       lcd.print(min);
     }
    // 
  
  
}

void DisplayTemp(int temp)

{
      if(flag2){
        double celcius=((temp*4.5)-500)/10;
        double fahrenheit=(celcius*1.8)+32; 
        lcd.setCursor(10,1);
        lcd.print(fahrenheit);
        lcd.print("F");
        lcd.print(" ");
      }
  
      else{
          double celcius=((temp*4.5)-500)/10;
          lcd.setCursor(10,1);
          lcd.print(celcius);
          lcd.print("C");
          lcd.print(" ");
      } 
}

void snooze(){
  
  if(alarmHour==hour && alarmMin==min){
    if(val4 == 0){
      noTone(alarm);
      alarmMin = alarmMin+5;
      if(alarmMin>=60){
        alarmHour = alarmHour + 1;
        if(alarmHour>23){
          alarmHour = 0;
          }
          alarmMin = alarmMin % 60;
        }
    }
  }   
}

void alarmRing(){
  
    if(hour==alarmHour && min==alarmMin){
      tone(alarm, 1234);         
    }
}

void TempFlag()
{        
  flag2=~flag2;
}

void TimeFlag()  
{
  flag1=~flag1;  
}

void setTime()
{
  flag3=~flag3;
}

void setClock(){

  if(flag3){
  
    min=min+1;
  
  }
  
}
