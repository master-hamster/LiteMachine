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
#include <EAnalogInput.h>
//#include <EBeeper.h>
#include <EButton.h>
#include <ELED.h>
#include <ERGBLED.h>
#include <ETimer.h>
#include <Events.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define SMALL_LIGHT_PIN      3
#define BIG_LIGHT_PIN        4        
#define MOTION_SENSOR_PIN    5
#define MODE_BUTTON_PIN      11
#define LIGHT_SENSOR_PIN     12

#define RED_LED_PIN      7
#define GREEN_LED_PIN    8
#define BLUE_LED_PIN     9


enum BoxMode {
        bmLightOff,
        bmLowLight,
        bmFullLight
};


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	int parseEvent();

        void switchToMode(BoxMode newMode);
        void setLightOff();
        void setLowLight();
        void setFullLight();


	oid_t timerID;
        oid_t bigLightID;
        oid_t smallLightID;
        oid_t modeButtonID;
        oid_t motionSensorID;
        oid_t lightSensorID;
        oid_t stateIndicatorID;
        
	ETimer timer;
        ELED smallLight;
        ELED bigLight;
        ERGBLED stateIndicator;
        EButton modeButton;
        EButton motionSensor;
        EAnalogInput lightSensor;
private:
        BoxMode currentMode;
        BoxMode lastMode;
        int currentLightLevel;
        bool motionDetected;

};
        
void MyApplication::init()
{
	timerID        = timer.init( 0, 0, true );
        bigLightID     = bigLight.init( BIG_LIGHT_PIN );
        smallLightID   = smallLight.init( SMALL_LIGHT_PIN );
        modeButtonID   = modeButton.init( MODE_BUTTON_PIN );
        motionSensorID = motionSensor.init( MOTION_SENSOR_PIN ); 
        lightSensorID  = lightSensor.init( LIGHT_SENSOR_PIN );

	addObject( &timer );
	addObject( &bigLight );
	addObject( &smallLight );
	addObject( &modeButton );
	addObject( &motionSensor );
	addObject( &lightSensor );

        currentMode = bmLightOff;
        lastMode = bmLightOff;
        currentLightLevel = 0;
        motionDetected = false;
};

int MyApplication::parseEvent()
/*
  Handle next events:
        current light level measure
        motion detection
        mode change
*/
{
        switch ( currentMode ) {
        case bmLightOff: 
             
                break;
        case bmLowLight:
        
                break;
                
        case bmFullLight:
        
                break;
        }        
        
        
	if ( currentEvent.eventType == 0 ) {
		Serial.print("RESULT Temp:");
		Serial.print(currentEvent.eventData);
		Serial.print(" and again:");
		Serial.println(float(currentEvent.eventData) / 100);
		return 1;
	}
  return 0;
};


void MyApplication::switchToMode(BoxMode newMode)
{
          
}


void setAllOff()
{
};

void setLowLight()
{
};

void setFullLight()
{
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

    



 

