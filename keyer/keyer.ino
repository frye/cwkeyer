#include <LiquidCrystal.h>

#define BAUD_DURATION          100              //mSec
#define INTERBAUD_DURATION     BAUD_DURATION*1
#define INTERLETTER_DURATION   BAUD_DURATION*2    //extra time after a baud
#define DIT_DURATION           BAUD_DURATION
#define DAH_DURATION           BAUD_DURATION*3

enum{
  IDLE,
  DIT,  
  DAH,
  PAUSE,
};

int dit,dah;
int state;

LiquidCrystal lcd(7,8,9,10,11,12);

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
  lcd.begin(16,2);
  lcd.setCursor(3,0);
  lcd.print("W7GE Keyer");
  lcd.setCursor(4,1);
  lcd.print("Welcome!");
  delay(2000);
}

void readDit(){
  if (digitalRead(A1)) dit=1; else dit=0;
}

void readDah() {
  if (digitalRead(A2)) dah=1; else dah=0;
}

void contact(int state) {
  if (state) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
// the loop function runs over and over again forever
void loop() {
   switch(state){
    case IDLE:
      readDit();
      if(dit) {
        state = DIT;              
      }
      else{
        delayMicroseconds(30);
        readDah();
        if(dah) {
          state = DAH;              
        }
      }      
    break;

    case DIT:
      contact(1);
      delay(DIT_DURATION);
      contact(0);
      delay(INTERBAUD_DURATION);
      //now, if dah is pressed go there, else check for dit
      readDah();
      if(dah){
        state = DAH;        
      }
      else{
        //read dit now
        readDit();
        if(dit) {
          state = DIT;              
        } 
        else {
          delay(INTERLETTER_DURATION);
          state = IDLE;
        }
      }        
    break;

    case DAH:
      contact(1);
      delay(DAH_DURATION);
      contact(0);
      delay(INTERBAUD_DURATION);
      //now, if dit is pressed go there, else check for dah
      readDit();
      if(dit){
        state = DIT;        
      }
      else{
        //read dit now
        readDah();
        if(dah) {
          state = DAH;              
        }
        else {
          delay(INTERLETTER_DURATION);
          state = IDLE;
        }
      }        
    break;
  }//switch   
  delay(1);
}
