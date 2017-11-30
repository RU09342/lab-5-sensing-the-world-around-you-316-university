# RGB LED 

RGB LED-based real-time feedback is supported for three MSP430 microcontrollers. The MSP430FR2311, MSP430G2553, and MSP430F5529 each have a code implementation that can be utilized. The ADC inputs for each microprocessor is P1.5, P2.5, and P6.5 respectively. The code is designed with an input range from GND to 3.3v. The ADC will complete a conversion at a rate of 1 HZ and will update the color of the RGB LED between Blue and Red based on the input voltage. As it stands right now, input near GND will make the LED appear bue, while input near 3.3v will make the LED appear red.

The specific boards that were chosen were chosen because they were the cheapest options. Since this code is not running any very tasking code it should run smoothly on even a low-end processor. Thats why price was taken into consideration, as extra cost is not neccessary to run this code.

# LCD Display

The LCD display is built to run on the MSP430FR6989 development board. The ADC input to the MSP430FR6989 is P8.6. At the moment the LCD screen is set to output whether the voltage input is greater or less than half of the reference range (0-3.3v). However this can be easily modified in the ADC interrupt depending on what specific application the user is looking to imnplement. Simply changing the ADC12MEM0 conversion to useable data and displaying what is desired using the ShowChar() function.

The MSP430FR6989 was chosen specifically for its LCD display.
