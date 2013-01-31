/*
MH 2013-01-19
Initial template

Basic Light controller 
- PIR Motion Sensor
- State Button
- Light Sensor

- Small Light Output
- Big Light Output
 

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
#include <EApplication.h>
#include <OneWire.h>
#include <DallasTemperature.h>



#define DEBUG_BASELITEBOX

//Inputs
//Digital Inputs
#define MOTION_SENSOR_PIN    1
#define MODE_BUTTON_PIN      2
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
#define evLightOff            132
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
        void testOutput( const oid_t OutputDeviceID );
        
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
        currentLightLevel = 0;
        motionDetected = false;

      	motionTimerID  = motionTimer.init( 10000, evMotionTimeout, false, false );
        bigLightID     = bigLight.init( BIG_LIGHT_PIN, PORT_REVERSE_MODE );
        smallLightID   = smallLight.init( SMALL_LIGHT_PIN, PORT_REVERSE_MODE );
        modeButtonID   = modeButton.init( MODE_BUTTON_PIN, true );   
            modeButton.setEvents( evStateButton, evLightOff );
        motionSensorID = motionSensor.init( MOTION_SENSOR_PIN, true ); 
            motionSensor.setEvents( evMotionDetected );
        lightSensorID  = lightSensor.init( LIGHT_SENSOR_PIN );
// set indicator port and reverse flag for all indicators
        indicatorLightOffID  = indicatorLightOff.init( RED_LED_PIN, PORT_REVERSE_MODE );
        indicatorLowLightID  = indicatorLowLight.init( BLUE_LED_PIN, PORT_REVERSE_MODE );
        indicatorFullLightID = indicatorFullLight.init( GREEN_LED_PIN, PORT_REVERSE_MODE );
        


	addObject( &motionTimer );
	addObject( &modeButton );
	addObject( &motionSensor );

	addObject( &smallLight );
	addObject( &bigLight );

        addObject( &indicatorLightOff );
        addObject( &indicatorLowLight );
        addObject( &indicatorFullLight );
        
	//addObject( &lightSensor );

        currentState = bsLightOff;
//        lastState    = bsLightOff;
        
//   check all outputs by direct control
//        indicatorLightOff.on();  delay(1000);  indicatorLightOff.off();   delay(1000);     
//        indicatorLowLight.on();  delay(1000);  indicatorLowLight.off();   delay(1000);     
//        indicatorFullLight.on(); delay(1000);  indicatorFullLight.off();  delay(1000);     
//        smallLight.on();         delay(1000);  smallLight.off();          delay(1000);     
//        bigLight.on();           delay(1000);  bigLight.off();            delay(1000);      
//delay(3000);

// another type of checking via events
//       testOutput(indicatorLightOffID); 
//       testOutput(indicatorLowLightID); 
//       testOutput(indicatorFullLightID); 
//       testOutput(smallLightID); 
//       testOutput(bigLightID); 
};


void MyApplication::testOutput(const oid_t OutputDeviceID) 
// Send evTurnOn, delay 1S, send evTurnOff, delay 1S;
{
        Serial.print(F("Check output ID:"));
        Serial.println(OutputDeviceID);
        currentEvent.destinationID = OutputDeviceID;        
        currentEvent.eventType = evTurnOn;
        handleEvent();
        delay(1000);
        currentEvent.eventType = evTurnOff;
        handleEvent();
        delay(1000);
        
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
        case evLightOff:
                setLightOff();
                break;
	}
};


void MyApplication::setNextState()
{
#ifdef DEBUG_BASELITEBOX
        Serial.print(F("Change mode from "));
        Serial.println(currentState);
        Serial.print( F("Free memory:"));
        Serial.println( freeRam() );

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
        Serial.println(F(" bsLightOff"));
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
        Serial.println(F(" bsLowLight"));
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
        Serial.println(F(" bsFullLight"));
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
        Serial.println(F(" bsMotionDetected"));
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



int freeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


//====================================================END OF MyApp definition
MyApplication mainApp;

void setup()
{
	Serial.begin(9600);
	Serial.println(F("BaseLiteBox Loading.."));
 
	mainApp.init();
	Serial.println(F("Current objects:"));
	mainApp.printNames();
	Serial.println(F("Loading done! Check hardware..."));

        // test all objects
        mainApp.sendTestEvent( evTurnOn, evTurnOff,  1000, 1000 );
	Serial.println(F("Output Testing done!"));

        // Set initial state to all light off
        mainApp.setLightOff();

};


void loop()
{ 
	if (mainApp.getEvent()) {  
#ifdef DEBUG_BASELITEBOX
		mainApp.printEvent();
#endif
		mainApp.parseEvent();
		mainApp.handleEvent();
	}
	mainApp.idle();
};

    



 

