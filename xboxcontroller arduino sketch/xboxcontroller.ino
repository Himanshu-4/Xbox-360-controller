#include<XInput.h>
#include<PS2X_lib.h>

// now we have our buttondatabyte and we have to extract the key press from here 
uint16_t buttons;
uint16_t last_buttons =0;
// defination of our buttons 
#define Select         (buttons & 1)
#define Start          ((buttons>>3)  & 1)
#define pad_up         ((buttons>>4)  & 1)
#define pad_right      ((buttons>>5)  & 1)
#define pad_down       ((buttons>>6)  & 1)
#define pad_left       ((buttons>>7)  & 1)
#define L2             ((buttons>>8)  & 1)
#define R2             ((buttons>>9)  & 1)
#define L1             ((buttons>>10) & 1)
#define R1             ((buttons>>11) & 1)
#define Green          ((buttons>>12) & 1)
#define RED            ((buttons>>13) & 1)
#define BLUE           ((buttons>>14) & 1)
#define PINK           ((buttons>>15) & 1)




#define ANALOG_DEADZONE 25
   int leftJoyX ;
   int leftJoyY ;
   int rightJoyX ;
   int rightJoyY ;
 
int prev_left_joyx, prev_left_joyy ,prev_right_joyx, prev_right_joyy;

PS2X ps2x;

 bool gamepadInitialized = false;

const boolean UseLeftJoystick   = true ;  // set to true to enable left joystick
const boolean InvertLeftYAxis   = false;  // set to true to use inverted left joy Y

const boolean UseRightJoystick  = true ;  // set to true to enable right joystick
const boolean InvertRightYAxis  = false;  // set to true to use inverted right joy Y

const boolean UseTriggerButtons = true;   // set to false if using analog triggers

#define ADC_Max  1023  // 10 bit

// Joystick Pins
#define  Pin_LeftJoyX   A1
#define  Pin_LeftJoyY   A0
#define Pin_RightJoyX   A3
#define Pin_RightJoyY   A2

#define left_motor 6
#define right_motor 5



void setup(){
 pinMode(left_motor,OUTPUT);
 pinMode(right_motor, OUTPUT);
 pinMode(9,INPUT);// L3
 pinMode(8,INPUT); //r3
	XInput.setJoystickRange(0, ADC_Max); 
XInput.setAutoSend(false);  // Wait for all controls before sending
// Set callback function. Function must have a 'void' return type
	// and take a single uint8_t as an argument
	XInput.setReceiveCallback(rumbleCallback);

	XInput.begin();

	uint8_t error = ps2x.config_gamepad(15,16,10,14,false, false);

	if (!error)
	{
		gamepadInitialized = true;
	}


}


void loop(){
	if (gamepadInitialized)
	{
		ps2x.read_gamepad();
		buttons  = ps2x.ButtonDataByte();
   leftJoyX = analogRead(Pin_LeftJoyX);
   leftJoyY = analogRead(Pin_LeftJoyY);
   rightJoyX = analogRead(Pin_RightJoyX);
   rightJoyY = analogRead(Pin_RightJoyY);
 

    }
if(buttons != last_buttons || leftJoyX != prev_left_joyx || leftJoyY != prev_left_joyy 
|| rightJoyX != prev_right_joyx || rightJoyY != prev_right_joyy )
{
// Set XInput buttons
	XInput.setButton(BUTTON_A, Green);
	XInput.setButton(BUTTON_B, RED);
	XInput.setButton(BUTTON_X, BLUE);
	XInput.setButton(BUTTON_Y, PINK);

	XInput.setButton(BUTTON_LB, L1);
	XInput.setButton(BUTTON_RB, R1);

	XInput.setButton(BUTTON_BACK, Select);
	XInput.setButton(BUTTON_START, Start);

	XInput.setButton(BUTTON_L3, digitalRead(9));
	XInput.setButton(BUTTON_R3, digitalRead(8));

	XInput.setDpad(pad_up, pad_down, pad_left, pad_right);

    XInput.setButton(TRIGGER_LEFT, L2);
	XInput.setButton(TRIGGER_RIGHT, R2);
    
    	// Set left joystick
	if (UseLeftJoystick == true) {
		

		// White lie here... most generic joysticks are typically
		// inverted by default. If the "Invert" variable is false
		// then we need to do this transformation.
		if (InvertLeftYAxis == false) {
			leftJoyY = ADC_Max - leftJoyY;
		}

		XInput.setJoystick(JOY_LEFT, leftJoyX, leftJoyY);
	}

	// Set right joystick
	if (UseRightJoystick == true) {
		

		if (InvertRightYAxis == false) {
			rightJoyY = ADC_Max - rightJoyY;
		}

		XInput.setJoystick(JOY_RIGHT, rightJoyX, rightJoyY);
	}

	// Send control data to the computer
	XInput.send();

last_buttons = buttons ;
prev_left_joyx = leftJoyX;
prev_left_joyy  = leftJoyY;
prev_right_joyx = rightJoyX;
prev_right_joyy  = rightJoyY;
}
}

void rumbleCallback(uint8_t packetType) {
	// If we have an LED packet (0x01), do nothing
	if (packetType == (uint8_t) XInputReceiveType::LEDs) {
		return;
	}

	// If we have a rumble packet (0x00), see our rumble data on the LED
	else if (packetType == (uint8_t) XInputReceiveType::Rumble) {
		uint8_t rumbleValueLeft  = XInput.getRumbleLeft();
        uint8_t rumbleValueRight =  XInput.getRumbleRight();
   analogWrite(right_motor , rumbleValueRight);
   analogWrite(left_motor  , rumbleValueLeft);
    }
}
