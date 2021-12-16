/*
   Measure A/C current and switch on LED when current is increasing 

  I use this circuit to detect if a machine (drilling machine, sander, chopsaw,...)
  is ON and if it is ON to switch on a vacuum cleaner)

  By measuring A/C, the measured value from the shields (I use ACS712, ZMCT103C) moves
  around the refence value 511. Analog Inputs have a value between 0 ... 1023. The shields dividing the value
  by two, to be able to measure A/C.

  The alogritm is simple:
  setup
  - measure the reference value by the connected shield on analog port (measurement count: 500)
  - dtermine the average reference value (sum of measuremnts divided by measurement counts)
  -> reference value

  loop
  - measure the value from analog port (measurement count: 500)
  - substract the refence value
  - if the value is less one (now, you have the negative sinus wave) multiply by -1 to get a positve value
  - sum the measured values
  - determine the average reference value (sum of measuremnts divided by measurement counts)
  -> the average measured value is less than threshold value 10 for ACS712 and less than threshold value 30 for ZMCT103C

  the amount of 500 measurements shouldn´t encounter problems, the arduino takes up to 9,700 measurements per second

  The circuit:
    LED
      LED anode (+) connect to pin 8, LED cathode (-) connect to 220 ohm resistor, resisor connect to : GND
    current measurement shield, like ACS712, ZMCT103C
      OUT: A0, VCC: 5V, GND: GND

  Created 15 December 2021
  By Mark Ross

  I am using these current sensor shields:
  
  ACS712 5A 
   * 5A range Current Sensor Module ACS712
   * 1.The current sensor chips: ACS712ELC-05B
   * 2.Pin 5V power supply, on-board power indicator
   * 3.The module can measure the positive and negative 5 amps, corresponding to the analog output 185mV/A
   * 4.No test current through the output voltage is VCC/2
   * 5.PCB board size: 31x13 (mm)

  ZMCT103C 5A
   * On-board micro precision current transformer
   * Onboard sampling strength
   * The module can measure AC currents less than 5A, the corresponding analog output 5A/5mA
   * Rated Input Current: 5A
   * Rated output current: 5mA
   * PCB size: 18.3 x17 (mm)
   * Change: 1000: 1
   * Linear range: 0 ~ 10A (100 ohms)
   * Linearity: 0.2%
   * Precision: 0.2
   * Uses Insulation Voltage: 3000 V Measure
   * Sealing material: epoxy resin
   * Operating temperature: -40 ℃ - 70 ℃

@section license License

 This file is part of the DetectCurrentForSwitching.

 DetectCurrentForSwitching is free software: you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 DetectCurrentForSwitching is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with DetectCurrentForSwitching. If not, see
 <http://www.gnu.org/licenses/>.
*/

int AnalogIn = 0; // Port connecting current measure shield OUT, like ACS712, ZMCT103C
int ledPin = 8; // Pin for connectin LED
float referenceValue = 0; // init reference value
float analogValue = 0; // init analog value


void setup() {
  Serial.begin(9600); // Start Serial connect
  referenceValue = referenceValueBy(500); //get reference value (value between 510 - 520)
  Serial.print("\t Reference value: ");
  Serial.println(referenceValue);
  //indicate one time to show circuit is starting
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);

}

void loop() {

  analogValue = averageAnalogValueBy(500); //get analog value (idle 10 for ACS712, up to 30 for ZMCT103C)
  Serial.print("\t analog value: ");
  Serial.println(analogValue);
  // The analog idle value differs by shield type and component tolerances
  // 10 seems to be a good threshold value for ACS712
  // 30 seems to be a good threshold value for ZMCT103C

  if (analogValue > 10) {
    digitalWrite(ledPin, HIGH); // indicate that the machine is ON
  } else {
    digitalWrite(ledPin, LOW); // indicate that the machine is OFF
  }

  delay(1000); // wait one second befor measuring again

}

/*
   referenceValueBy: measures reference value by given measurement cycles
   and returns the average value
   @param measurements number of measure cycles
   @return reference value
*/
float referenceValueBy(int measurements) {

  int counter;
  int analogValue = 0;
  float referenceValue = 0;
  //
  for (counter = 1; counter < measurements; counter++) {
    analogValue = analogRead(AnalogIn);
    referenceValue = referenceValue + analogValue; // sum values
  }
  referenceValue = referenceValue / measurements; // calulate average
  return referenceValue;
}

/*
   averageAnalogValueBy: measures average analog value by given measurement cycles
   and returns the average value
   @param measurements number of measure cycles
   @return average analog value
   
*/
float averageAnalogValueBy(int measurements) {
  int counter;
  float currentValue = 0; // measured value without reference value 
  float sum = 0; // sum of current values
  float averageValue = 0; // calculated average value 

  for (counter = 1; counter < measurements; counter++) {
    analogValue = analogRead(AnalogIn); 
    currentValue = analogValue - referenceValue;

    // by measuring A/C you also will get negative value
    // we don´t want to have these: multiply value by -1 
    // to get positive values
    if (currentValue < 0) {
      currentValue = currentValue * -1;
    }
    sum = sum + currentValue;
  }

  averageValue = sum / measurements;

  return averageValue;
}
