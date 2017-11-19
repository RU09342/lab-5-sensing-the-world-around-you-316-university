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

  /*
  //Initialize ADC
  ADCCTL0 |= BIT4;
  ADCCTL1 |= (BIT4+BIT3);
  ADCCTL2 |= BIT5;
  ADCMCTL0 |= (BIT2+BIT0);
  ADCIE |= BIT0;
*/

  //TRICTL|= TRIPM_1|TRIEM_1;

  ADCCTL0 |= ADCSHT_8 | ADCON;                             // ADCON, S&H=16 ADC clks
  ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
  ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
  ADCMCTL0 |= ADCINCH_5;                                   // A1 ADC input select; Vref=AVCC
  ADCIE |= ADCIE0;


  //Initialize timer
  TB1CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
  TB1CCTL1=OUTMOD_7; //Red LED reset/set mode
  TB1CCTL2=OUTMOD_7;//Green LED reset/set mode
  TB1CCR0 = 1024;//Timer B, capture control 0 set to 50000 cycles
  TB1CCR1 = 0xf0; //Red PWM
  TB1CCR2 = 0xf0; //Blue PWM
  TB1CTL |= TBSSEL__SMCLK | MC__UP | (BIT6+BIT7);//select SMCLK, up mode

  // Configure GPIO
  Init_GPIO();

  PM5CTL0 &= ~LOCKLPM5;                    // Disable the GPIO power-on default high-impedance mode
                                           // to activate 1previously configured port setting

  __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
int timing=0;
#pragma vector = TIMER1_B0_VECTOR
__interrupt void turn_on_ADC(void)
{
    switch (timing)
    {
    case 0: timing++;

        break;
    case 1: timing = 0;
    ADCCTL0|= ADCENC | ADCSC;
        break;
    }

}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            while(!(UCA0IFG&UCTXIFG));       //Use this line in ADC interrupt
            TB1CCR1 = ADCMEM0;
            TB1CCR2 = 1024- ADCMEM0;
            break;
        default:
            break;
    }
}

void Init_GPIO() //P1.5=>A5
{
    P1SEL0|=BIT5;
    P1SEL1|=BIT5;
    P2DIR |= BIT0+BIT1;
    P2SEL0|= BIT0+BIT1;
    P2SEL1 &= ~(BIT0+BIT1);
}
