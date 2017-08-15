/*
  HRXL MB7380 Maxbotix (5m sensor)

  Pin 1:  White   --> Temperature Sensor Connection. Leave unconnected if an external temperature sensor is not used.

  Pin 2:  Orange  --> Pulse Width Output.                      
                      
  Pin 3:  Brown   --> Analog Voltage Output.                       
                      
  
  Pin 4:  Green   --> Ranging Start/Stop.                                            
                      
                                          
  Pin 5:  Blue  -->   Serial Output, TTL. Maxbotix serial output is inverted.
  
  Pin 6:  Red   -->   Positive Power, VCC: operates from 2.7V to 5.5V DC
                      For best operation, sensor requires DC be free from electrical noise.
                      For situation of known dirty electrical power, a 100uF capacitor placed at the sensor pins
                      between V+ and GND will typically correct the electrical noise.
                      
  Pin 7:  Black -->   Ground pin. DC return and circuit common ground.
*/

/*
  Circuit 
  =======
  Sonar 1     <->   Arduino/Breadboard
  ... Red           common positive power
  ... Black         common ground
  ... Green         D2
  ... Orange        D10

  Sonar 2     <->   Arduino/Breadboard
  ... Red           common positive power
  ... Black         common ground
  ... Green         D3
  ... Orange         D11
*/

const byte sonar1_Enable = 2;           // Sonar 1 Enable pin, digital
const byte sonar1_PWInput = 10;        // Sonar 1 PW pin
long sonarDist = 0;

void setup() {
  Serial.begin(9600); 
  pinMode(sonar1_Enable,OUTPUT);   
  //digitalWrite(sonar1_Enable, LOW);  
  digitalWrite(sonar1_Enable, HIGH);  
  delay(1000);
}

void loop() {      
  //digitalWrite(sonar1_Enable, HIGH);  
  Serial.println();
  Serial.print("Sensor: ");
  sonarDist = pulseIn(sonar1_PWInput,HIGH);
  sonarDist = sonarDist/10;
  Serial.print(sonarDist);
  Serial.println(" cm");
  //digitalWrite(sonar1_Enable, LOW);  
  delay(100); 
}

