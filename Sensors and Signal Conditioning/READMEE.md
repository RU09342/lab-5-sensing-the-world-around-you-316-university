# Photodiode Circuit

The photodiode circuit was based on converting the current generated from the photodiode into a voltage that could be read through the ADC on a microcontroller. The main challenge of this design was figuring out what level of gain to enact on the signal. This was a challenge because the datasheet was not available for reference. Therefore, the gain was determined through experimental means. The first stage of gain would be completed by a transimpedance amplifier. This type of amplifier is designed to convert currents into a voltage, and the gain is reffereed to in Ohms. This is because the output of the circuit is equal to the current runnning through the photodiode times the feedback resistance. In our circuit, we chose this gain experimentally to be 1Meg. This ended up giving the circuit a range of outputs from 0-260 mV. This lead to another stage of gain being implemented. We needed a gain of 12.7 in the second amplifier to reach the full range of outputs, 0-3.3v. A noninverting op amp was chosen. The gain of a noniverting op amp is shown below. 

![Alt Text] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/d38178dd1da01924fd206abea9f29353d073e5ef)

This led to the choice of 120k and 10.27k resistances for R2 and R1 respectively. Overall, this gain took the photodiode from 0-3.3v over the full range of light that we were testing with. Below the full schematic for the photodiode circuit can be seen. This circuit gave us full resolution using 3.3v for the reference in our ADC software.

![Alt Text] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/04561c4e15a1862645e0f5efc73e8e7ff1aa6e1f)

# Photoresistor Circuit

The challenge of the photoresistor was cnverting a changing resistance value into a changing voltage value. This was accomplished by using a voltage divider. Experimentally, the range of resistance that the photoresistor held was determined to be from about 100 Ohms up to 70k Ohms. A series of calculations was done using the voltage divider equation. Eventually, a 1K resistor was decided upon to be the other resistor in the divider. These calculations are shown below. The photoresistor was chosen to be resistor number two, because in our calculations this led to a greater output voltage range.

![Alt Text] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/631f1b9ff926e9f2a3a7a41ab9f498e7a32f03e7)

The output of the divider provided a voltage range of 0.3v-3.25v. This was an acceptable range of voltage and didn't require any additional adding or subtracting circuitry. A buffer was included in the final design in order to eliminate any innacuracies that could be caused by the input impedance of the MSP430. The final circuit can be seen below.

![Alt Text] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/67a56208a209ed9995270ad24527805fdf2a6fc2)

# Phototransistor Circuit

The phototransistor was a new challenge. The phototransistor acted almost like a photodiode that was connected to the base of a BJT. This causes the transistor to pass more current as it receives more light. After trying multiple different configurations, it was decided that biasing the base of the transistor, and taking the reading at the collector of the transistor provided the best results. Using the shown configuration, the output voltage ranged from 0.5v-2.85v. Although a portion of the 0-3.3v range was cutoff, this still provided sufficient precision for the ADC. The circuit created can be seen below. 

![Alt Text] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/5b46f3d064915b35b5a7631896e02e24be1cd90d)

# Software

The software for each sensor was identical. This is because the sensors were each designed to provide a reasonable range of the 0-3.3v range that was utilized by each ADC. This allowed any sensor to be hooked up to the MSP430 and they acted similarly. The ADC was configured with AVSS and AVCC as the reference voltages. Timing of the ADC was handled by a timer by setting bit 0 of the ADCCTL register whenever a voltage conversion was desired. The conversion was always handled by ADCMEM0. Whenever an ADC conversion was finished the value received was sent out through the transmit buffer at 9600 baud. This software was written for every board.

The only differences between each board was only register names and configuration. All of the concepts for code structure stayed exactly the same.
