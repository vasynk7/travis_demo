
#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
//volatile int Flag=0;
//volatile int Flag1=0;
volatile int pressure=0;
volatile int belt=0;

int main()
{
 //   DDRD&=~(1<<2);
 //   DDRD&=~(1<<3);
    DDRB&=~(1<<5);
    DDRC&=~(1<<3);
    // for switch
   // PORTD|=(1<<2);
   // PORTD|=(1<<3);
    PORTB|=(1<<5); // pressure check
    PORTC|=(1<<3); // Seat belt check
    // led direction
    DDRB|=(1<<0);// Green led
    DDRB|=(1<<1);// Red led
    DDRB|=(1<<2); // Buzzer
    DDRC|=(1<<2);

    EICRA|=(1<<ISC00);
    EIMSK|=(1<<INT0);
    EICRA|=(1<<ISC01);
    EIMSK|=(1<<INT1);
   // PCIE0|=(1<<0);
    PCICR|=(1<<0);
    PCMSK0|=(1<<5);
    PCICR|=(1<<1);
    PCMSK1|=(1<<3);

    sei();

    while(1)
    {
       /* if(Flag==1)
        {
            PORTB|=(1<<0);
            _delay_ms(1000);
            Flag=0;
        }
        else if(Flag1==1)
        {
            PORTB|=(1<<1);
            _delay_ms(1000);
            Flag1=0;
        }
        */
        if(pressure==1)
        {
           if(belt==1)
        {
            PORTB|=(1<<0);
            //PORTC|=(1<<2);
           // _delay_ms(1000);
            //Flag3=0;

        }
        else
        {
         PORTB|=(1<<2);
         PORTB&=~(1<<0);
         PORTB&=~(1<<1);


        }
        }
        else
        {
           PORTB&=~(1<<0);
           PORTB|=(1<<1);
           PORTB&=~(1<<2);
        }
    }
    return 0;
}
  /*  ISR(INT0_vect)
    {
        Flag=1;
    }
    ISR(INT1_vect)
    {
        Flag1=1;
    }
    */
     ISR(PCINT0_vect)
    {
        pressure=1;
    }
    ISR(PCINT1_vect)
    {
        belt=1;
    }
