/*
MH 2013-01-19
Initial template

Basic Light controller 
- PIR Motion Sensor
- State Button
- Light Sensor

- Small Light Output
- Big Light Output
- 

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


enum BoxState {
        bsLightOff,
        bsLowLight,
        bsFullLight,
        bsMotionDetected
};

#define evStateButton         131
#define evMotionTimeout       133



//=====================================================================
class MyApplication : public EApplication {
public:
	void init();
	void parseEvent();

        void setNextState();

//        void switchToState(BoxState newState); 
        void setLightOff();
        void setLowLight();
        void setFullLight();
        void setMotionDetected();

	oid_t motionTimerID;

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
        
        ELED indicatorLightOff;
        ELED indicatorLowLight;
        ELED indicatorFullLight;

        
	ETimer motionTimer;
        ELED smallLight;
        ELED bigLight;
//        ERGBLED stateIndicator;
private:
        BoxState currentState;
//        BoxState lastState;
        int currentLightLevel;
        bool motionDetected;

};
        
void MyApplication::init()
{
	motionTimerID  = motionTimer.init( 10000, evMotionTimeout, false );
        bigLightID     = bigLight.init( BIG_LIGHT_PIN );
        smallLightID   = smallLight.init( SMALL_LIGHT_PIN );
        modeButtonID   = modeButton.init( MODE_BUTTON_PIN, true );   
          modeButton.setEvents( evStateButton );
        motionSensorID = motionSensor.init( MOTION_SENSOR_PIN, true ); 
        lightSensorID  = lightSensor.init( LIGHT_SENSOR_PIN );

        indicatorLightOffID  = indicatorLightOff.init( RED_LED_PIN );
        indicatorLowLightID  = indicatorLowLight.init( GREEN_LED_PIN );
        indicatorFullLightID = indicatorFullLight.init( BLUE_LED_PIN );


	addObject( &motionTimer );
	addObject( &modeButton );
	addObject( &motionSensor );

        // actualy not needed but Ordnung!
	addObject( &bigLight );
	addObject( &smallLight );
	//addObject( &lightSensor );

        currentState = bsLightOff;
//        lastState    = bsLightOff;
        currentLightLevel = 0;
        motionDetected = false;
};

void MyApplication::parseEvent()
/*
  Handle next events:
        current light level measure
        motion detection
        mode change
*/
{
        switch ( currentEvent.eventType ){
	case evStateButton:
                setNextState();
                break;
        case evMotionDetected:
                if ( currentState == bsLightOff )  {
                        setMotionDetected();
                } else if ( currentState == bsMotionDetected )  {
                        motionTimer.start();
                }       
                break;
        case evMotionTimeout:
                if ( currentState == bsMotionDetected ) {
                        setLightOff();
                }      
                break;
	}
};


void MyApplication::setNextState()
{
#ifdef DEBUG_BASELITEBOX
        Serial.print("Change mode from ");
        Serial.println(currentState);
#endif          
        switch ( currentState ) {
        case bsLightOff: 
                setLowLight();
                break;
        case bsLowLight:
                setFullLight();
                break;
        case bsFullLight:
                setLightOff();        
                break;
        case bsMotionDetected:
                setFullLight();
                break;
        }        
}

/*
void MyApplication::switchToState(BoxState newState)
{
          
}
*/

void MyApplication::setLightOff()
{
        currentState = bsLightOff;
#ifdef DEBUG_BASELITEBOX
        Serial.println(" bsLightOff");
#endif
        bigLight.off();
        smallLight.off();
// set mode indication
        indicatorLightOff.on();        
        indicatorLowLight.off();
        indicatorFullLight.off();
};

void MyApplication::setLowLight()
{
        currentState = bsLowLight;
#ifdef DEBUG_BASELITEBOX
        Serial.println(" bsLowLight");
#endif
        bigLight.off();
        smallLight.on();
// set mode indication
        indicatorLightOff.off();        
        indicatorLowLight.on();
        indicatorFullLight.off();
};

void MyApplication::setFullLight()
{
        currentState = bsFullLight;
#ifdef DEBUG_BASELITEBOX
        Serial.println(" bsFullLight");
#endif
        bigLight.on();
        smallLight.off();
// set mode indication
        indicatorLightOff.off();        
        indicatorLowLight.off();
        indicatorFullLight.on();
};

void MyApplication::setMotionDetected()
{
        currentState = bsMotionDetected;
#ifdef DEBUG_BASELITEBOX
        Serial.println(" bsMotionDetected");
#endif
        motionTimer.start();
//select light level based on current light level        
        bigLight.on();
        smallLight.on();
// set mode indication
        indicatorLightOff.on();        
        indicatorLowLight.on();
        indicatorFullLight.on();
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

    



 

