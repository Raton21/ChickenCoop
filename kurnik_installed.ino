#define photocellPin 7                 //photocell connected to digital 6
#define relayDown 2                     // enable relay for downwards direction
#define relayUp 3                       // enable relay for upwards direction
#define topSwitchPin 4                  // top switch is connected to pin 4
#define bottomSwitchPin 5               // bottom switch is connected to pin 5


#define night_level 1000
#define day_level 1015

int photocellReading;                            // analog reading of the photocel
                               // top switch var for reading the pin status                  // top switch var for reading the pin delay/debounce status
bool topSwitchState;                    // top switch var for to hold the switch state
                            // bottom switch var for reading the pin status               // bottom switch var for reading the pin delay/debounce status
bool bottomSwitchState;                 // bottom switch var for to hold the switch state


// ************************************** the setup **************************************
void setup(void) {
Serial.begin(9600); 
// coop door
// coop door motor
pinMode (relayUp, OUTPUT);            // enable relay pin = output
pinMode (relayDown, OUTPUT);          // enable relay pin = output

// coop door switches
// bottom switch
pinMode(bottomSwitchPin, INPUT_PULLUP);                  // set bottom switch pin as input
// digitalWrite(bottomSwitchPin, HIGH);              // activate bottom switch resistor
// top switch
pinMode(topSwitchPin, INPUT_PULLUP);                     // set top switch pin as input
// digitalWrite(topSwitchPin, HIGH);                 // activate top switch resistor
pinMode(photocellPin, INPUT);
}
// ************************************** functions **************************************
// operate the coop door
// photocel to read levels of exterior light


void stopCoopDoorMotor(){
  digitalWrite (relayUp, HIGH);      
  digitalWrite (relayDown, HIGH);       
}

void closeCoopDoorMotor() {
    bottomSwitchState = digitalRead(bottomSwitchPin); 
  Serial.println("chci jet dolu");
    if(bottomSwitchState == 1){
                  delay(1000000);

      while(bottomSwitchState == 1){
          Serial.println("jedu dolu");

            bottomSwitchState = digitalRead(bottomSwitchPin);
        digitalWrite (relayUp, LOW);     
        digitalWrite (relayDown, HIGH);
        }
      stopCoopDoorMotor();
    }
}

void openCoopDoorMotor(){
    topSwitchState = digitalRead(topSwitchPin);
   Serial.println("chci jet nahoru");
    if(topSwitchState == 1){
        while(topSwitchState == 1){
           Serial.println("jedu nahoru");
          topSwitchState = digitalRead(topSwitchPin);
      digitalWrite(relayUp, HIGH);       
      digitalWrite(relayDown, LOW); 
        }
         
      stopCoopDoorMotor();
    }
}

void doCoopDoor(){
  photocellReading = digitalRead(photocellPin);
  if (photocellReading  == 1) {  
    closeCoopDoorMotor();                     
  }
  else {            
    openCoopDoorMotor();                       
  }
  Serial.print("hodnota:");
  Serial.println(photocellReading);
}
// ************************************** the loop **************************************
void loop(){
  doCoopDoor();
  delay(30000);
}
