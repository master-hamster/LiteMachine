
/*
MH 2013-01-19
Initial template

Basic Light controller 
- PIR Motion Sensor
- Mode Button
- Light Sensor

- Small Light Output
- Big Light Output
- RGB Mode Led

*/

//#include <Arduino.h>
//#include <EAButton.h>
//#include <EAEventButton.h>
//#include <EAnalogInput.h>
//#include <EBeeper.h>
//#include <EButton.h>
//#include <ELED.h>
#include <ETimer.h>
#include <Events.h>


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	int parseEvent();

	oid_t timerID;
        oid_t bigLightID;
        oid_t smallLightID;
        oid_t modeButtonID;
        
//        EButton modeButton;
	ETimer timer;

};
        
void MyApplication::init()
{

	// case 1. passive thermometer, answer only when someone ask
	timerID = timer.init(0,0,true);

	addObject(&timer);
};

int MyApplication::parseEvent()
{
	if (currentEvent.eventType==0) {
		//We've got temperature event - let's print data
		Serial.print("RESULT Temp:");
		Serial.print(currentEvent.eventData);
		Serial.print(" and again:");
		Serial.println(float(currentEvent.eventData) / 100);
		return 1;
	}
  return 0;
};

//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println("BaseLiteBox Loading..");

 
	mainApp.init();
	Serial.println("Current objects:");
	mainApp.printNames();

	Serial.println("Loading done!");
};


void loop()
{ 
	if (mainApp.getEvent()) {  
		mainApp.printEvent();
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
};

    



 

