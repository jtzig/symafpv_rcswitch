// Using PWM read code from
// http://rcarduino.blogspot.com/ 
//

#define THROTTLE_SIGNAL_IN 0 // INTERRUPT 0 = DIGITAL PIN 2 - use the interrupt number in attachInterrupt
#define THROTTLE_SIGNAL_IN_PIN 2 // INTERRUPT 0 = DIGITAL PIN 2 - use the PIN number in digitalRead

#define OUT_PIN 6

#define NEUTRAL_THROTTLE 1500 // this is the duration in microseconds of neutral throttle on an electric RC Car

volatile boolean recording = false;
volatile int nThrottleIn = NEUTRAL_THROTTLE; // volatile, we set this in the Interrupt and read it in loop so it must be declared volatile
volatile int oThrottleIn = NEUTRAL_THROTTLE;
volatile unsigned long ulStartPeriod = 0; // set in the interrupt
volatile boolean bNewThrottleSignal = false; // set in the interrupt and read in the loop
// we could use nThrottleIn = 0 in loop instead of a separate variable, but using bNewThrottleSignal to indicate we have a new signal 
// is clearer for this first example

void setup()
{
  // tell the Arduino we want the function calcInput to be called whenever INT0 (digital pin 2) changes from HIGH to LOW or LOW to HIGH
  // catching these changes will allow us to calculate how long the input pulse is
  attachInterrupt(THROTTLE_SIGNAL_IN,calcInput,CHANGE);
  pinMode(13, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);
// Serial.begin(9600); 
} 

void loop()
{
 // if a new throttle signal has been measured, lets print the value to serial, if not our code could carry on with some other processing
 if(bNewThrottleSignal)
 {

  if ((nThrottleIn>1500)&&(recording==false)) {

//start recording

  digitalWrite(13, HIGH);
  digitalWrite(OUT_PIN, HIGH);
  delay(2000);              // wait for 2 seconds
  digitalWrite(13, LOW);
  digitalWrite(OUT_PIN, LOW);
  recording = true;
  //Serial.println(1);  

  } else {

//stop recording

  digitalWrite(13, HIGH);
  digitalWrite(OUT_PIN, HIGH);
  delay(100);              // wait for 10th of a second
  digitalWrite(13, LOW);
  digitalWrite(OUT_PIN, LOW);

  recording = false;
  //Serial.println(0);  
    
  }

   // Serial.println(nThrottleIn);  

   // set this back to false when we have finished
   // with nThrottleIn, while true, calcInput will not update
   // nThrottleIn
   bNewThrottleSignal = false;
 }


 
}

void calcInput()
{
  // if the pin is high, its the start of an interrupt
  if(digitalRead(THROTTLE_SIGNAL_IN_PIN) == HIGH)
  { 
    // get the time using micros - when our code gets really busy this will become inaccurate, but for the current application its 
    // easy to understand and works very well
    ulStartPeriod = micros();
  }
  else
  {
    // if the pin is low, its the falling edge of the pulse so now we can calculate the pulse duration by subtracting the 
    // start time ulStartPeriod from the current time returned by micros()
    if(ulStartPeriod && (bNewThrottleSignal == false))
    {

      oThrottleIn = nThrottleIn;
      nThrottleIn = (int)(micros() - ulStartPeriod);
      ulStartPeriod = 0;

      // tell loop we have a new signal on the throttle channel
      // we will not update nThrottleIn until loop sets
      // bNewThrottleSignal back to false

      //Serial.println(nThrottleIn);    
    
     if (((nThrottleIn - oThrottleIn)>100)||((nThrottleIn - oThrottleIn)<-100)) bNewThrottleSignal = true;
   
    }
  }
}
