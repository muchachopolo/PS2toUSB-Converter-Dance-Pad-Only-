#include "PS2X_lib.h"
#include "UnoJoy.h"

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;

bool TrianglePin = false;
bool SquarePin = false;
bool CrossPin = false;
bool CirclePin = false;

bool LeftPin = false;
bool UpPin = false;
bool RightPin = false;
bool DownPin = false;

bool StartPin =  false;
bool SelectPin =  false;

bool Temp = false;
void setup(){
   error = ps2x.config_gamepad(9,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
   setupUnoJoy();
}

void loop(){
   
 if(error == 1) //skip loop if no controller found
  return;
  
 if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller 
   
   if(ps2x.ButtonPressed(GREEN_FRET))
     Serial.println("Green Fret Pressed");
   if(ps2x.ButtonPressed(RED_FRET))
     Serial.println("Red Fret Pressed");
   if(ps2x.ButtonPressed(YELLOW_FRET))
     Serial.println("Yellow Fret Pressed");
   if(ps2x.ButtonPressed(BLUE_FRET))
     Serial.println("Blue Fret Pressed");
   if(ps2x.ButtonPressed(ORANGE_FRET))
     Serial.println("Orange Fret Pressed");
     

    if(ps2x.ButtonPressed(STAR_POWER))
     Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
     Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
     Serial.println("DOWN Strum");
  
 
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");

    
    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
        Serial.print("Wammy Bar Position:");
        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
 }

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START)){                   //will be TRUE as long as button is pressed
        StartPin = true;
        digitalWrite(LED_BUILTIN, StartPin);
    }
    if(ps2x.Button(PSB_SELECT))
    {
        SelectPin = true;
        digitalWrite(LED_BUILTIN, SelectPin);
    }
         
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
        UpPin = true;
        digitalWrite(LED_BUILTIN, UpPin);
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       RightPin = true;
       digitalWrite(LED_BUILTIN, RightPin);
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       LeftPin = true;
       digitalWrite(LED_BUILTIN, LeftPin);
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       DownPin = true;
       digitalWrite(LED_BUILTIN, DownPin);
      }   
  
    
      vibrate = ps2x.Analog(PSAB_BLUE);
                                              
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN)){
          TrianglePin = true;
          digitalWrite(LED_BUILTIN, TrianglePin);
        }
    }   
         
    
    if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
    {
      CirclePin = true;
      digitalWrite(LED_BUILTIN, CirclePin);
    }
         
    if(ps2x.ButtonReleased(PSB_PINK)){             //will be TRUE if button was JUST released
         SquarePin = true;
         digitalWrite(LED_BUILTIN, SquarePin);
    }
    
    if(ps2x.NewButtonState(PSB_BLUE)){           //will be TRUE if button was JUST pressed OR released
         CrossPin = true;
         digitalWrite(LED_BUILTIN, CrossPin);
    }
    
    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
    }
 }
 dataForController_t controllerData = getControllerData();
 setControllerData(controllerData);
 delay(50);
 resetButtons();
}

dataForController_t getControllerData(void){
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  controllerData.triangleOn = UpPin;
  controllerData.circleOn = DownPin;
  controllerData.squareOn = LeftPin;
  controllerData.crossOn = RightPin;
  controllerData.dpadUpOn = TrianglePin;
  controllerData.dpadDownOn = CirclePin;
  controllerData.dpadLeftOn = SquarePin;
  controllerData.dpadRightOn = CrossPin;
  
  controllerData.startOn = StartPin;
  controllerData.selectOn = SelectPin;
  
  // And return the data!
  return controllerData;
}

void resetButtons()
{
  TrianglePin = false;
  SquarePin = false;
  CrossPin = false;
  CirclePin = false;
  
  LeftPin = false;
  UpPin = false;
  RightPin = false;
  DownPin = false;
  
  StartPin =  false;
  SelectPin =  false;
  
  Temp = false;
  digitalWrite(LED_BUILTIN, LOW);
}

