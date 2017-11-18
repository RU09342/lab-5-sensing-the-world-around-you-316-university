#include <msp430.h> 
//Bryan Regn & Chris Iapicco
//Created: October 23rd, 2017
//Last Updated: October 23rd, 2017
/**
 * main.c
 */

void Init_GPIO();

int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                // Stop watchdog timer

  ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
  ADC12CTL1 = ADC12SHP;                     // Use sampling timer
  ADC12IE = 0x01;                           // Enable interrupt
  ADC12MCTL0 |= ADC12INCH_5;
  ADC12CTL0 |= ADC12ENC;

  //Initialize timer
  TA2CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
  TA2CCR0 = 4096;//Timer B, capture control 0 set to 50000 cycles
  TA2CCTL1 |= OUTMOD_7;
  TA2CCTL2 |= OUTMOD_7;
  TA2CCR1 |= 500;
  TA2CCR2 |= 500;
  TA2CTL |= TASSEL__SMCLK | MC__UP | (BIT6+BIT7);//select SMCLK, up mode

  // Configure GPIO
  Init_GPIO();


  __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
int timing=0;
#pragma vector = TIMER2_A0_VECTOR
__interrupt void turn_on_ADC(void)
{
    switch (timing)
    {
    case 0: timing++;

        break;
    case 1: timing = 0;
    ADC12CTL0|= 0x03;
        break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{//Band name: Problem with the code!!!!
    switch(__even_in_range(ADC12IV,34))
    {
    case  0: break;                           // Vector  0:  No interrupt
    case  2: break;                           // Vector  2:  ADC overflow
    case  4: break;                           // Vector  4:  ADC timing overflow
    case  6:                                  // Vector  6:  ADC12IFG0
        while(!(UCA1IFG&UCTXIFG));       //Use this line in ADC interrupt
         TA2CCR1 = ADC12MEM0;
         TA2CCR2 = 4096 - ADC12MEM0;
    case  8: break;                           // Vector  8:  ADC12IFG1
    case 10: break;                           // Vector 10:  ADC12IFG2
    case 12: break;                           // Vector 12:  ADC12IFG3
    case 14: break;                           // Vector 14:  ADC12IFG4
    case 16: break;                           // Vector 16:  ADC12IFG5
    case 18: break;                           // Vector 18:  ADC12IFG6
    case 20: break;                           // Vector 20:  ADC12IFG7
    case 22: break;                           // Vector 22:  ADC12IFG8
    case 24: break;                           // Vector 24:  ADC12IFG9
    case 26: break;                           // Vector 26:  ADC12IFG10
    case 28: break;                           // Vector 28:  ADC12IFG11
    case 30: break;                           // Vector 30:  ADC12IFG12
    case 32: break;                           // Vector 32:  ADC12IFG13
    case 34: break;                           // Vector 34:  ADC12IFG14
    default: break;
    }
}

void Init_GPIO() //P1.5=>A5
{
    P2DIR |= BIT4+BIT5;
    P2SEL |= BIT4+BIT5;
    P6SEL |= BIT5;
}
