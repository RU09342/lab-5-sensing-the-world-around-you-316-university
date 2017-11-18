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

  ADCCTL0 |= ADCSHT_8 | ADCON;                             // ADCON, S&H=16 ADC clks
  ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
  ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
  ADCMCTL0 |= ADCINCH_5;                                   // A1 ADC input select; Vref=AVCC
  ADCIE |= ADCIE0;


  //Initialize timer
  TB0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
  TB0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
  TB0CTL |= TBSSEL__SMCLK | MC__UP | (BIT6+BIT7);//select SMCLK, up mode

  // Configure GPIO
  Init_GPIO();

  PM5CTL0 &= ~LOCKLPM5;                    // Disable the GPIO power-on default high-impedance mode
                                           // to activate 1previously configured port setting
  // Configure UART pins
  P1SEL0 |= BIT6 | BIT7;                    // set 2-UART pin as second function

  // Configure UART
  UCA0CTLW0 |= UCSWRST;
  UCA0CTLW0 |= UCSSEL__SMCLK;

  // Baud Rate calculation
  // 1000000/(16*9600) = 104.1667
  // Fractional portion = 0.1667
  // User's Guide Table 17-4: UCBRSx = 0x20
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BR0 = 6;                             // 1000000/16/9600
  UCA0BR1 = 0x00;
  UCA0MCTLW = 0b0010000010000001;

  UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
int timing=0;
#pragma vector = TIMER0_B0_VECTOR
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
            UCA0TXBUF = ADCMEM0;
            ADCIFG=0x00;
            break;
        default:
            break;
    }
}

void Init_GPIO() //P1.5=>A5
{
    P1SEL0|=BIT5;
    P1SEL1|=BIT5;
}
