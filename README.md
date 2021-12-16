# arduinoCurrentDetector
Measure A/C current and switch on LED when current is increasing

I use this circuit to detect if a machine (drilling machine, sander, chopsaw,...) is ON and if it is ON to switch on a vacuum cleaner)

By measuring A/C, the measured value from the shields (I use ACS712, ZMCT103C) moves around the refence value 511. Analog Inputs have a value between 0 ... 1023. 

The shields dividing the value by two, to be able to measure A/C.

## The alogritm is simple:
  
  ### setup
  
  - measure the reference value by the connected shield on analog port (measurement count: 500)
  - dtermine the average reference value (sum of measuremnts divided by measurement counts)
  -> reference value
  
  ### loop
  
  - measure the value from analog port (measurement count: 500)
  - substract the refence value
  - if the value is less one (now, you have the negative sinus wave) multiply by -1 to get a positve value
  - sum the measured values
  - determine the average reference value (sum of measuremnts divided by measurement counts)
  -> the average measured value is less than threshold value 10 for ACS712 and less than threshold value 30 for ZMCT103C
  the amount of 500 measurements shouldn´t encounter problems, the arduino takes up to 9,700 measurements per second
  
## The circuit:
  * LED
      LED anode (+) connect to pin 8, LED cathode (-) connect to 220 ohm resistor, resisor connect to : GND
  * Current measurement shield, like ACS712, ZMCT103C
      OUT: A0, VCC: 5V, GND: GND
  
##
  * Created 15 December 2021 
  * By Mark Ross

## Sensors 
  I am using these current sensor shields:
  
### ACS712 5A 
   * 5A range Current Sensor Module ACS712
   * 1.The current sensor chips: ACS712ELC-05B
   * 2.Pin 5V power supply, on-board power indicator
   * 3.The module can measure the positive and negative 5 amps, corresponding to the analog output 185mV/A
   * 4.No test current through the output voltage is VCC/2
   * 5.PCB board size: 31x13 (mm)
  
 ### ZMCT103C 5A
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

## section license License

 This file is part of the arduinoCurrentDetector.

 arduinoCurrentDetector is free software: you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 arduinoCurrentDetector is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with DetectCurrentForSwitching. If not, see
 <http://www.gnu.org/licenses/>.

