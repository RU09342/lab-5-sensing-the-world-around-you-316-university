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
  TA0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
  TA0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
  TA0CTL |= TASSEL__SMCLK | MC__UP | (BIT6+BIT7);//select SMCLK, up mode



 // Configure UART pins
  //P1SEL |= BIT6 | BIT7;                    // set 2-UART pin as second function

  // Configure UART
  UCA1CTLW0 |= UCSWRST;
  UCA1CTLW0 |= UCSSEL__SMCLK;

  // Configure GPIO
  Init_GPIO();

  // Baud Rate calculation
  // 1000000/(16*9600) = 104.1667
  // Fractional portion = 0.1667
  // User's Guide Table 17-4: UCBRSx = 0x20
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA1BR0 = 6;
  UCA1BR1 = 0;// 1000000/16/9600                // UBRSx=0 UCBRFx=8
  UCA1MCTL = 0b10000001;

  UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
  //UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM3_bits|GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
int timing=0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void turn_on_ADC(void)
{
    switch (timing)
    {
    case 0: timing++;
   //UCA1TXBUF = 0xff;
    //P1OUT ^= BIT0;
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
{
    switch(__even_in_range(ADC12IV,34))
    {
    case  0: break;                           // Vector  0:  No interrupt
    case  2: break;                           // Vector  2:  ADC overflow
    case  4: break;                           // Vector  4:  ADC timing overflow
    case  6:                                  // Vector  6:  ADC12IFG0
        while(!(UCA1IFG&UCTXIFG));       //Use this line in ADC interrupt
         UCA1TXBUF = ADC12MEM0;
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
    P6SEL |= BIT5;
    P4SEL |= BIT4;
    //P1DIR |= BIT0;
}

