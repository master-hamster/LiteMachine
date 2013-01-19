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
//#include <ERGBLED.h>
#include <ETimer.h>
#include <Events.h>

#include <OneWire.h>
#include <DallasTemperature.h>



#define DEBUG_BASELITEBOX


//Inputs
//Digital Inputs
#define MOTION_SENSOR_PIN    4
#define MODE_BUTTON_PIN      5
//Analog Inputs
#define LIGHT_SENSOR_PIN     3

//Outputs
#define SMALL_LIGHT_PIN      8
#define BIG_LIGHT_PIN        9        

#define RED_LED_PIN      10
#define GREEN_LED_PIN    11
#define BLUE_LED_PIN     12


enum BoxMode {
        bmLightOff,
        bmLowLight,
        bmFullLight,
        bmMotionDetected
};

#define evChangeMode         131


//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	int parseEvent();

        void setNextMode();

        void switchToMode(BoxMode newMode); //?????
        void setLightOff();
        void setLowLight();
        void setFullLight();

	oid_t timerID;

        oid_t motionSensorID;
        oid_t modeButtonID;
        oid_t lightSensorID;

        oid_t bigLightID;
        oid_t smallLightID;
        oid_t indicatorLightOffID;
        oid_t indicatorLowLightID;
        oid_t indicatorFullLightID;

        EButton modeButton;
        EButton motionSensor;
        EAnalogInput lightSensor;
        
	ETimer timer;
        ELED smallLight;
        ELED bigLight;
//        ERGBLED stateIndicator;
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
        modeButtonID   = modeButton.init( MODE_BUTTON_PIN, true );
        motionSensorID = motionSensor.init( MOTION_SENSOR_PIN, true ); 
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
	if ( (currentEvent.eventType == evKeyPressed) && (currentEvent.sourceID == modeButtonID ) ) {
                setNextMode();
		return 1;
	}
  return 0;
};


void MyApplication::setNextMode()
{
        Serial.print("Change mode from ");
        Serial.println(currentMode);
          
        switch ( currentMode ) {
        case bmLightOff: 
                setLowLight();
                break;
        case bmLowLight:
                setFullLight();
                break;
        case bmFullLight:
                setLightOff();        
                break;
        }        
}


void MyApplication::switchToMode(BoxMode newMode)
{
          
}


void MyApplication::setLightOff()
{
        currentMode = bmLightOff;
        bigLight.off();
        smallLight.off();
};

void MyApplication::setLowLight()
{
        currentMode = bmLowLight;
        bigLight.off();
        smallLight.on();
};

void MyApplication::setFullLight()
{
        currentMode = bmFullLight;
        bigLight.on();
        smallLight.off();
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

    



 

