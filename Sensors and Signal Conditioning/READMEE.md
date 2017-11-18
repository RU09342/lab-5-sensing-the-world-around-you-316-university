# Photodiode Circuit

The photodiode circuit was based on converting the current generated from the photodiode into a voltage that could be read through the ADC on a microcontroller. The main challenge of this design was figuring out what level of gain to enact on the signal. This was a challenge because the datasheet was not available for reference. Therefore, the gain was determined through experimental means. The first stage of gain would be completed by a transimpedance amplifier. This type of amplifier is designed to convert currents into a voltage, and the gain is reffereed to in Ohms. This is because the output of the circuit is equal to the current runnning through the photodiode times the feedback resistance. In our circuit, we chose this gain experimentally to be 1Meg. This ended up giving the circuit a range of outputs from 0-260 mV. This lead to another stage of gain being implemented. We needed a gain of 12.7 in the second amplifier to reach the full range of outputs, 0-3.3v. A noninverting op amp was chosen. The gain of a noniverting op amp is shown below. 

![Gain of a noninverting op amp] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/d38178dd1da01924fd206abea9f29353d073e5ef)

This led to the choice of 120k and 10.27k resistances for R2 and R1 respectively. Overall, this gain took the photodiode from 0-3.3v over the full range of light that we were testing with. Below the full schematic for the photodiode circuit can be seen. This circuit gave us full resolution using 3.3v for the reference in our ADC software.

![Photodiode sensing circuit] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/04561c4e15a1862645e0f5efc73e8e7ff1aa6e1f)

# Photoresistor Circuit

![Photoresistor sensing circuit] (https://github.com/RU09342/lab-5-sensing-the-world-around-you-316-university/commit/04561c4e15a1862645e0f5efc73e8e7ff1aa6e1f)

