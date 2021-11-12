#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

#define relayDown 2                     // enable relay for downwards direction
#define relayUp 3                       // enable relay for upwards direction
#define topSwitchPin 4                  // top switch is connected to pin 4
#define bottomSwitchPin 5               // bottom switch is connected to pin 5
#define buttonUp 6                      // external buttons for controlling door manually
#define buttonDown 7

// Convert normal decimal numbers to binary coded decimal

byte decToBcd(byte val){
    return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
    return( (val/16*10) + (val%16) );
}

                               // top switch var for reading the pin status                  // top switch var for reading the pin delay/debounce status
bool topSwitchState;                    // top switch var for to hold the switch state
                            // bottom switch var for reading the pin status               // bottom switch var for reading the pin delay/debounce status
bool bottomSwitchState;                 // bottom switch var for to hold the switch state
// ****  0 - closed   ***  1 - open 
bool doorState;    //what state is the door right now (changes when hitting end switches)
bool doorTarget =1;   //target state of door 
// ************************************** the setup **************************************
void setup() {
      Wire.begin(); 
    // set time: seconds, minutes, hours, day, date, month, year
   // setDS3231time(30,40,18,6,30,10,21);
Serial.begin(9600); 
// coop door
// coop door motor
pinMode (relayUp, OUTPUT);            // enable relay pin = output
pinMode (relayDown, OUTPUT);          // enable relay pin = output

//     ***coop door switches***
// bottom switch
pinMode(bottomSwitchPin, INPUT_PULLUP);                  // set bottom switch pin as input

// top switch
pinMode(topSwitchPin, INPUT_PULLUP);                     // set top switch pin as input

}
// ************************************** coop door functions **************************************
// operate the coop door
// photocel to read levels of exterior light


void stopCoopDoorMotor(){
  digitalWrite (relayUp, HIGH);      
  digitalWrite (relayDown, HIGH);       
}

void closeCoopDoorMotor() {
  
        digitalWrite (relayUp, LOW);     
        digitalWrite (relayDown, HIGH);
        }

void openCoopDoorMotor(){
    Serial.print("jedu nahoru");
      digitalWrite(relayUp, HIGH);       
      digitalWrite(relayDown, LOW); 
        }

void doCoopDoor(){
 if (doorState == doorTarget){
   stopCoopDoorMotor();
 }
 if ((doorTarget == 1) && (doorState == 0)){
  // Serial.print("chci nahoru");
   openCoopDoorMotor();
 }
 if ((doorTarget == 0) && (doorState == 1)){
   closeCoopDoorMotor();
   Serial.print("chci dolu");
 }
}
void coopDoorDetect(){
  if (digitalRead(topSwitchPin) == 0){
    doorState = 1;
  }
  if (digitalRead(bottomSwitchPin) == 0){
    doorState = 0;
  }  

}
void buttonInput(){
  if (digitalRead(buttonUp) == 1){
    doorTarget = 1;
  }
  if (digitalRead(buttonDown) == 1){
    doorTarget = 0;
  }

}
// *********************************** DST time reated functions ***********************
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Monday, 7=Sunday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void timeControl(){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    Serial.print("hour:");
    Serial.println(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    
    Serial.print(":");
    if (minute<10){
        Serial.print("0");
    }
    Serial.print(minute, DEC);
   /* Serial.print(":");
    if (second<10){
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Day of week: ");
    switch(dayOfWeek){
        case 1:
            Serial.println("Sunday");
            break;
        case 2:
            Serial.println("Monday");
            break;
        case 3:
            Serial.println("Tuesday");
            break;
        case 4:
            Serial.println("Wednesday");
            break;
        case 5:
            Serial.println("Thursday");
            break;
        case 6:
            Serial.println("Friday");
            break;
        case 7:
            Serial.println("Saturday");
            break;
    }
    */
    if (minute == 0) {
      if (hour == 8 ) {
        doorTarget = 1;
      }
      if (hour == 20 ) {
        doorTarget = 0;
      }
    }
}

// ************************************** the loop **************************************
void loop(){
  coopDoorDetect();
  timeControl();
  buttonInput();
  doCoopDoor();
  Serial.print("stavDveri");
  Serial.println(doorState);
  Serial.print("cilDveri");
  Serial.println(doorTarget);
  delay(200);
}
