/*
  HRXL MB7380 Maxbotix (5m sensor)

  Pin 1:  White   --> Temperature Sensor Connection. Leave unconnected if an external temperature sensor is not used.

  Pin 2:  Orange  --> Pulse Width Output. 
                      ===================
                      Scale factor of 1 uS per mm. 
  
  Pin 3:  Brown   --> Analog Voltage Output. 
                      =====================
                      After ~50ms power up init, voltage is set to low.
                      One sensor has completed a range reading, the voltage on this pin is set to voltage
                      corresponding to the latest measured distance. 
                      (rc: take note, is this same behaviour for Serial and Pulse width?)
                      (rc: for Analog, if Pin 4 is set to low, analog values MUST BE IGNORED)
                      Scale factor of Vcc/5120 per 1-mm. Distance output with a 5mm resolution.
                      Analog voltage counts (0-1023), multiply number of counts by 5 to yield in mm.
  
  Pin 4:  Green   --> Ranging Start/Stop. This pin is internally pulled high. 
                      ==================
                      If ths pin is left unconnected or held high, the sensor will continually measure and output the range data.
                      If held low, the sensor will stop ranging
                      Bring high for 20uS or longer to command a range reading.

                      Filtered Range Data: 
                      When pin 4 is left high on the sensors, the sensors will continue to range. The data that is output
                      includes a filter for increased accuracy. The sensors will output the range based on recent range
                      information. The filter does not affect the speed at which data is made available to the user but
                      instead allows for more consistent range information to be presented. For sensor specific timing
                      and filter information refer to pages 8 and 9.
                      
                      Real-time Range Data: When pin 4 is low and then brought high, the sensor will operate in real time
                      and the first reading output will be the range measured from this first commanded range reading. 
                      When the sensor tracks that the RX pin is low after each range reading, and then the RX pin is brought
                      high, unfiltered real time range information can be obtained. For timing information please refer
                      to pages 8 and 9 of datasheet.
                                          
  Pin 5:  Blue  -->   Serial Output. TTL outputs. 
                      ============
                      Output is ASCII capital "R" followed by four ASCII character digits representing range in mm followed by
                      a carriage return (ASCII 13). 
                      Min range is 300m, Max range is 4999mm. Range of 5000mm means no target dtected.
                      9600, 8N1. Because data is presented in a binary data format, serial output is most accurate.
                      
  Pin 6:  Red   -->   Positive Power, VCC: operates from 2.7V to 5.5V DC
                      For best operation, sensor requires DC be free from electrical noise.
                      For situation of known dirty electrical power, a 100uF capacitor placed at the sensor pins
                      between V+ and GND will typically correct the electrical noise.
                      
  Pin 7:  Black -->   Ground pin. DC return and circuit common ground.



*/

/*    rc notes:
  test analog, pwm and serial to see which is more reliable
  online sources says analog reads are sensitive and to refer to Arduino forum, but need to check sonar model, when comments were made
  advice from online is to do average.
  apparently PW method is most accurage, however in datasheet, serial is supposed to be most reliable.

  final decision on which mthd depends on reliability and what pins we have available on the MCU

  Also, if using multiple maxbotix ultrasonic sensors, is there a need to range them in sequence, rather than leave them ranging continuously.

*/

/*
  Circuit 
  =======

  Sonar 1     <->   Arduino/Breadboard
  ... Red           common positive power
  ... Black         common ground
  ... Green         D2
  ... Brown         A0

  Sonar 2     <->   Arduino/Breadboard
  ... Red           common positive power
  ... Black         common ground
  ... Green         D3
  ... Brown         A1


*/


//Sonar 1
const byte sonar1_analogInput = 0;      // Sonar 1 Analog pin
const byte sonar1_Enable = 2;           // Sonar 1 Enable pin, digital
const byte sonar2_analogInput = 1;      // Sonar 2 Analog pin
const byte sonar2_Enable = 3;           // Sonar 2 Enable pin, digital

long sonarDist = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sonar1_Enable,OUTPUT); 
  pinMode(sonar2_Enable,OUTPUT);   
  Sonar_RangingMode(sonar1_Enable,0);
  Sonar_RangingMode(sonar2_Enable,0);
}

void loop() {  

  Sonar_RangingMode(sonar1_Enable,1);
  Serial.println();
  Serial.print("Sensor 1: ");
  Sonar_AnalogRead(sonar1_analogInput);
  Sonar_RangingMode(sonar1_Enable,0);
  delayMicroseconds(500); // a slight delay gives more consistent and accurate results
                          // so far 500 microseconds seems best, but it also depends on distance sensor is away from another sensor
                          
  Sonar_RangingMode(sonar2_Enable,1);
  Serial.print("Sensor 2: ");
  Sonar_AnalogRead(sonar2_analogInput);
  Serial.println();
  Sonar_RangingMode(sonar2_Enable,0);
  
  delay(500);  
}

void Sonar_RangingMode(byte sonarPin, byte modeEnable) {
  if(modeEnable) {
    digitalWrite(sonarPin, HIGH);
  } else {
    digitalWrite(sonarPin, LOW);
  }  
}

void Sonar_AnalogRead(byte analogPin) {
/*
  Scale factor is (Vcc/1024) per 5mm. A 5V supply yields ~4.9mV/5mm
  Arduino analog pin goes from 0 to 1023, so the value has to be multiplied by 5 to get range in mm
*/
  sonarDist = analogRead(analogPin)/2; //dist in cm
  Serial.print(sonarDist);
  Serial.println(" cm");
  
  /*sonar1Dist = analogRead(sonarPin)*5; // dist in mm
  Serial.print(sonarDist);
  Serial.println(" mm");
  */ 
}

