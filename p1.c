/*
 * p1.c
 *
 *  Created on: Oct 17, 2020
 *      Author: lenovo
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hour1=0;
unsigned char hour2=0;

void timer1_comp_init(void){
	TCCR1A|=(1<<FOC1A);
	TCCR1B=(1<<CS12)|(1<<CS10)|(1<<WGM12);
	TCNT1=0;
	OCR1A=1000;
	TIMSK|=(1<<OCIE1A);
	SREG|=(1<<7);

}
void int0_init(void){
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2);
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT0);
	SREG|=(1<<7);

}
void int1_init(void){
	DDRD&=~(1<<PD3);
	MCUCR=(1<<ISC11)|(1<<ISC10);
	GICR|=(1<<INT1);
	SREG|=(1<<7);
}
void int2_init(void){
	DDRB&=~(1<<PB2);
	PORTB|=(1<<PB2);
	MCUCSR&=~(1<<ISC2);
	GICR|=(1<<INT2);
	SREG|=(1<<7);
}
ISR(TIMER1_COMPA_vect){
	sec1++;
}
ISR(INT0_vect){
	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
	GIFR&=~(1<<INTF0);
}
ISR(INT1_vect){
	TCCR1B&=~(1<<CS12)&~(1<<CS11)&~(1<<CS10);
	GIFR&=~(1<<INTF1);
}
ISR(INT2_vect){
	TCCR1B|=(1<<CS12)|(1<<CS10);
	GIFR&=~(1<<INTF2);
}
void sec1_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA0);
	if(sec1>9){
		sec1=0;
		sec2++;
	}
	PORTC= (PORTC&0xf0)|(sec1&0x0f);
}
void sec2_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA1);
	if(sec2==6){
		sec2=0;
		min1++;
	}
	PORTC= (PORTC&0xf0)|(sec2&0x0f);
}
void min1_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA2);
	if(min1>9){
		min1=0;
		min2++;
	}
	PORTC= (PORTC&0xf0)|(min1&0x0f);
}
void min2_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA3);
	if(min2==6){
		min2=0;
		hour1++;
	}
	PORTC= (PORTC&0xf0)|(min2&0x0f);
}
void hour1_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA4);
	if(hour1>9){
		hour1=0;
		hour2++;
	}
	PORTC= (PORTC&0xf0)|(hour1&0x0f);
}
void hour2_display(void){
	PORTA&=0b11000000;
	PORTA|=(1<<PA5);
	PORTC= (PORTC&0xf0)|(hour2&0x0f);
}
int main(void){
	DDRC|=0x0f;
	PORTC&=0xf0;
	DDRA|=0b00111111;
	PORTA&=0b11000000;
	int0_init();
	int1_init();
	int2_init();
	timer1_comp_init();
	while(1){
		sec1_display();
		_delay_us(5);
		sec2_display();
		_delay_us(5);
		min1_display();
		_delay_us(5);
		min2_display();
		_delay_us(5);
		hour1_display();
		_delay_us(5);
		hour2_display();
		_delay_us(5);
	}

}
