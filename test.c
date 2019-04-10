#include <avr/io.h>

#include <util/delay.h>

#include <avr/interrupt.h>

 

#define SET_PWM_VALUE(value)  OCR0A = value

 

#define SET_BIT(PORT,PIN)  PORT |= (1<<PIN)

#define CLR_BIT(PORT,PIN)  PORT &= ~(1<<PIN)

 

#define START_ADC  ADCSRA |= (1<<ADSC)

#define ENABLE_ADC ADCSRA |= (1<<ADEN)

 

 

volatile uint8_t Engine_Flag=0;

volatile uint8_t Inc_Flag=0;

volatile uint8_t Dec_Flag=0;

 

volatile uint8_t entry=0;

volatile uint8_t entry1=0;

 

int  PWM_val=128;

uint16_t Adc_val=0;

 

void pins_init()

{

    SET_BIT(DDRD,PD6); // PD6 (OC0A)output

 

    CLR_BIT(DDRC,PC0); // Potentiometer input value

 

    CLR_BIT(DDRD,PD2); //Engine Switch

    SET_BIT(PORTD,PD2); //PULLUP Enabled

 

    CLR_BIT(DDRD,PD3); // switch1 for +ve

    SET_BIT(PORTD,PD3); //PULLUP Enabled

 

    CLR_BIT(DDRD,PD4); // Switch2 for -ve

    SET_BIT(PORTD,PD4); //PULLUP Enabled

 

    SREG |= (1<<7); //Global inetrrupt

 

    EICRA |= ((1<<ISC10)|(1<<ISC00)); //the falling edge will trigger  an interrupt

    EIMSK |= ((1<<INT1)|(1<<INT0));   //PD2=INT0,PD3=INT1;

 

 

    PCICR |= (1<<PCIE2);  // PD4 enabling the 3rd set of pins(16-23)

    PCMSK2 |= (1<<PCINT20); //enabling the interrupt for PCINT20

 

    SET_BIT(DDRD,PD7); //Engine Switch

 

 

}

 

void PWM_init()

{

    TCNT0 =0x00;  // give access to timer and counter

    TCCR0A |= ((1<<WGM00)|(1<<WGM01)|(1<<COM0A1)); // enabled the mode and effect

    TCCR0B |= ((1<<CS00)|((1<<CS01))); //clock and WGM02

}

void ADC_init()

{

    ENABLE_ADC;

    ADMUX |= ((1<<REFS0)); // reference voltage and left adjust

    ADCSRA |= ((1<<ADPS1)|(1<<ADPS2)); // division factor

}

 

uint16_t READ_ADC()

{

    START_ADC;

    while(ADCSRA & (1<<ADSC));

    return ADC;

}

 

 

int main(void)

{

    pins_init();

    PWM_init();

    ADC_init();

 

       while(1)

        {

        if(Engine_Flag==1)

        {

            SET_BIT(PORTD,PD7);

            Adc_val= READ_ADC();

            if(Adc_val<=750)

            {

            SET_PWM_VALUE(PWM_val);

            _delay_ms(50);

            }

            else

            {

                SET_PWM_VALUE(0);

            }

            Inc_Flag=0;

            Dec_Flag=0;

        }

        else

        {

            CLR_BIT(PORTD,PD7);

            SET_PWM_VALUE(0);

 

            if(Inc_Flag==1)

            {

                SET_BIT(PORTD,PD7);

                _delay_ms(1000);

                CLR_BIT(PORTD,PD7);

                PWM_val += 13;

                 Inc_Flag=0;

            }

 

 

            if(Dec_Flag==1)

            {

                 SET_BIT(PORTD,PD7);

                _delay_ms(1000);

                CLR_BIT(PORTD,PD7);

                 Dec_Flag=0;

                PWM_val -= 13;

            }

 

        }

 

    }

 

 

 

    return 0;

}
