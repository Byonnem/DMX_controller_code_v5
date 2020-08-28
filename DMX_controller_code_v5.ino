/* -- M.D.S 04-2020 --
  --- Code to be used with the DMX controller for QLC+ communication via MIDI. written and tested for ARDUINO MEGA 2560 ---
*/

//Declaring all constants
#define noteOn 144 //midi value for note on
const int Pinamount_faders = 8; //amount of faders on the PCB
const int FaderPins[Pinamount_faders] = {A0, A2 , A4   , A6  , A7 , A5 , A3 , A1}; //The pins to which the faders are connected to. The order makes for easy indexing (most left fader is fader 0, most right fader is 7)
int previous_value_faders[128]; //array with the previous values so only MIDI message is sent when a value changes

const int Buttonamount = 8; //amount of buttons
const int ButtonPins[Buttonamount] = {10, 11, 12, 13, 14, 15, 16, 17}; //digitalRead pin numbers for array of all buttons
bool previous_value_Button[Buttonamount];

void MIDImessage(byte command, byte data1, byte data2) { //function to send a midi command to Hairless
  Serial.write(command); //note on command
  Serial.write(data1); //note
  Serial.write(data2); //note velocity
}

void setup() {
  Serial.begin(256000); //initialize serial
  for (int i = 0; i < (sizeof(FaderPins) / sizeof(int)); i++) { //initialize all faders
    pinMode(FaderPins[i], INPUT);
  }
  for (int i = 0; i < (sizeof(ButtonPins) / sizeof(int)); i++) { //initialize all buttons
    pinMode(ButtonPins[i], INPUT);
  }
}

void loop() {
  for (int i = 0; i < Pinamount_faders; i++) { //check all faders
    int value = analogRead(FaderPins[i]) / 8; //convert the 1024 bit to 128
    if (value != previous_value_faders[i]) { //check if value is changed
      previous_value_faders[i] = value; //if value changed, then the new value is stored
      MIDImessage(noteOn, i, value); //send MIDI message with new value
    }
    else {}
  }
  for (int i = 0; i < Buttonamount; i++) {
    bool value = digitalRead(ButtonPins[i]);
    if (value != previous_value_Button[i]) { //check if value is changed
      previous_value_Button[i] = value; //if value changed, then the new value is stored
      MIDImessage(noteOn, i + Pinamount_faders, value); //sent MIDI message, key starts after array of the faders.
    }
  }// put your main code here, to run repeatedly:
}
