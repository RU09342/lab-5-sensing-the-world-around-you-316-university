#include <msp430.h> 
//Chris Iapicco
//Created: October 29th, 2017
//Last Updated: October 29th, 2017
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P2DIR|=BIT1+BIT5;
	P2SEL|=BIT1+BIT5;

	  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
	  ADC10CTL1 = INCH_5;                       // input A1
	  ADC10AE0 |= BIT5;                         // PA.1 ADC option select

	  TA1CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
	  TA1CCR0 = 1024;//Timer B, capture control 0 set to 50000 cycles
	  TA1CTL |= TASSEL_2  | MC_1 | (BIT6+BIT7);//select SMCLK, up mode
	  TA1CCTL1=OUTMOD_7;
	  TA1CCTL2=OUTMOD_7;
	  TA1CCR1=500;
	  TA1CCR2=500;

	  __bis_SR_register(LPM0_bits|GIE);         // Enter LPM3, interrupts enabled
	return 0;
}

int timing=0;
#pragma vector = TIMER1_A0_VECTOR
__interrupt void turn_on_ADC(void)
{
    switch (timing)
    {
    case 0: timing++;

        break;
    case 1: timing = 0;
    ADC10CTL0|= 0x03;
        break;
    }

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC10_VECTOR))) ADC10_ISR (void)
#else
#error Compiler not supported!
#endif
{
    //while(!(UCA0IFG&UCTXIFG));       //Use this line in ADC interrupt
    TA1CCR1=ADC10MEM;
    TA1CCR2=1024-ADC10MEM;
}
