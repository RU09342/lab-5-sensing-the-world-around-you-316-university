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
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
/*
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_5;                       // input A1
  ADC10AE0 |= BIT5;                         // PA.1 ADC option select
*/
  /*
  //Initialize ADC10
  ADC10CTL0 |= BIT4;
  ADC10CTL1 |= (BIT4+BIT3);
  ADC10MCTL0 |= (BIT2+BIT0);
  AD12CIE |= BIT0;
*/
  //Initialize timer
  TA0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
  TA0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
  TA0CTL = TASSEL_2 + MC_1 + TACLR;                  // SMCLK, upmode
  TA0CTL |= (BIT6+BIT7);//select SMCLK, up mode

  // Configure GPIO
  Init_GPIO();


  // Configure UART pins
  //P1SEL |= BIT6 | BIT7;                    // set 2-UART pin as second function

  // Configure UART
  UCA0CTL1 |= UCSWRST;
  UCA0CTL1 |= BIT7;

  // Baud Rate calculation
  // 1000000/(16*9600) = 104.1667
  // Fractional portion = 0.1667
  // User's Guide Table 17-4: UCBRSx = 0x20
  // UCBRFx = int ( (52.083-52)*16) = 1
  UCA0BR0 = 6;
  UCA0BR1 = 0;
  UCA0MCTL = 0b10000001;

  UCA0CTL0 &= ~UCSWRST;                    // Initialize eUSCI
  //UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits|GIE);         // Enter LPM3, interrupts enabled
  __no_operation();                         // For debugger
}
int timing=0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void turn_on_ADC10(void)
{
    switch (timing)
    {
    case 0: timing++;
    UCA0TXBUF = 0xf0;
    P1OUT^=BIT0;
        break;
    case 1: timing = 0;
    ADC10CTL0|=0x03;
    timing = 0;
    break;
    }

}

// ADC10 interrupt service routine
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
    UCA0TXBUF = ADC10MEM;
}

void Init_GPIO() //P1.5=>A5
{
    P1SEL|=BIT2;
    P1SEL2|=BIT2;
    P1DIR|=BIT0;
}
