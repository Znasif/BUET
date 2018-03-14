#include <avr/io.h>
#define F_CPU 160000UL
#include <util/delay.h>
#include <avr/interrupt.h>

/*
void drawCharRed(unsigned char *);
void drawCharGreen(unsigned char *);
void drawBlank();
*/

unsigned int result;
float volt;
unsigned int i,j;

unsigned short pos_x1=8, pos_y1=2;
unsigned short pos_x2=8, pos_y2=13;

unsigned int life1=3, life2=3, power1=6, power2=6;

unsigned int flag1=0, flag2=0;

unsigned bullet_count1=0, bullet_count2=0;

unsigned int bullet_start_index1=0, bullet_start_index2=0;

unsigned int num_bullet = 13;

unsigned int bullet_delay1=0, bullet_delay2=0;

unsigned int bullet_x1[230], bullet_x2[230], bullet_y1[230], bullet_y2[230];

int temp1, temp2;

int power1_pressed_time=0, power2_pressed_time=0; 

int power1_count=0, power2_count=0;

int player1_powerx[3], player1_powery[3], player2_powerx[3], player2_powery[3];

unsigned int power_delay1=0, power_delay2=0;

unsigned int power_enable1=-1, power_enable2=-1;

ISR(INT0_vect) {
	flag1 = 1;	
	GIFR |= 1<<INTF0;
}


ISR(INT1_vect) {
	flag2 = 1;
	GIFR |= 1<<INTF1;
}


int player1IsShooted(int pos_x1, int pos_y1, int bullet_x1, int bullet_y1)
{
	if((pos_x1==bullet_x1 && pos_y1==bullet_y1)
	|| (pos_y1-1==bullet_y1 && (pos_x1-1==bullet_x1 || pos_x1==bullet_x1 || pos_x1+1==bullet_x1)) 
	||(pos_y1-2==bullet_y1 && (pos_x1-2==bullet_x1 || pos_x1-1==bullet_x1 || pos_x1==bullet_x1 || pos_x1+1==bullet_x1 || pos_x1+2==bullet_x1))
	)
	{
		return 1;
	}
	return 0;
}



int player2IsShooted(int pos_x1, int pos_y1, int bullet_x1, int bullet_y1)
{
	if((pos_x1==bullet_x1 && pos_y1==bullet_y1)
	|| (pos_y1+1==bullet_y1 && (pos_x1-1==bullet_x1 || pos_x1==bullet_x1 || pos_x1+1==bullet_x1))
	||(pos_y1+2==bullet_y1 && (pos_x1-2==bullet_x1 || pos_x1-1==bullet_x1 || pos_x1==bullet_x1 || pos_x1+1==bullet_x1 || pos_x1+2==bullet_x1)))
	{
		return 1;
	}
	return 0;
}

void initializeVariables()
{
	
	pos_x1=8, pos_y1=2;
	pos_x2=8, pos_y2=13;

	life1=3, life2=3, power1=6, power2=6;

	flag1=0, flag2=0;

	bullet_count1=0, bullet_count2=0;

	bullet_start_index1=0, bullet_start_index2=0;

	num_bullet = 13;

	bullet_delay1=0, bullet_delay2=0;

	power1_pressed_time=0, power2_pressed_time=0;
	
	power1_count=0, power2_count=0;
	
	power_delay1=0, power_delay2=0;
	
	power_enable1=-1, power_enable2=-1;
}

void powerOfPlayer1(int pos_y1)
{
	for(int a=0; a<bullet_count2; a++)
	{
		if(pos_y1==bullet_y2[a])
		{
			bullet_count2--;
			bullet_x2[a] = bullet_x2[bullet_count1];
			bullet_y2[a] = bullet_y2[bullet_count1];
			a--;		
		}
	}	
}



int main(void)
{
		
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	
	
	DDRC=0xff;//positive row
	DDRA=0x0f;//negative column red
	DDRB=0xff;//negative column green
	
	ADCSRA = 0b10000100;
	
	
	
	GICR = (1<<INT1)|(1<<INT0);
	MCUCR = MCUCR | 0b00001010;
	sei();
	


    while(1)
    {
			unsigned short player1[] = {
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000
			};
			
			unsigned short player2[] = {
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000
			};
			/*
			unsigned short player1win[] = {
				0b0000000000000000,
				0b0000000110000000,
				0b0000001110000000,
				0b0000011110000000,
				0b0000110110000000,
				0b0001100110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0000000110000000,
				0b0001111111111000,
				0b0001111111111000,
				0b0000000000000000
			};
			*/
			unsigned short player1win[] = {
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0010001010111110,
				0b0011001010101010,
				0b0010101010101010,
				0b0010011010100010,
				0b0010001010100010,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000
			};
			
			
			/*
			unsigned short player2win[] = {
				0b0000000000000000,
				0b0111111111111110,
				0b0111111111111110,
				0b0000000000000110,
				0b0000000000000110,
				0b0000000000000110,
				0b0000000000000110,				
				0b0111111111111110,
				0b0111111111111110,
				0b0110000000000000,
				0b0110000000000000,
				0b0110000000000000,
				0b0110000000000000,				
				0b0111111111111110,
				0b0111111111111110,
				0b0000000000000000
			};*/
			
			unsigned short player2win[] = {
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0100010101000100,
				0b0100010101100100,
				0b0101010101010100,
				0b0101010101001100,
				0b0111110101000100,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000,
				0b0000000000000000
			};
			
		
		power_delay1++;
		power_delay2++;
			
		ADMUX = 0b01100100;
		
	    ADCSRA |= (1<<ADSC);
		while( ADCSRA & (1<<ADSC) )
			;
			
		result = ADCL;
		result = (result>>6) | (ADCH<<2);
		
		volt = result*5.0/1024;
		
		if (volt>4) {
			
			if(pos_x1>3) pos_x1--;
			
		} else if (volt<1) {
			
			if(pos_x1<12) pos_x1++;
		}
		
		
		
		ADMUX = 0b01100101;
		
		ADCSRA |= (1<<ADSC);
		while( ADCSRA & (1<<ADSC) )
		;
		
		result = ADCL;
		result = (result>>6) | (ADCH<<2);
		
		volt = result*5.0/1024;
		
		if (volt>4) {
			
			if(pos_y1<7) pos_y1++;
			
			} else if (volt<1) {
			
			if(pos_y1>2) pos_y1--;
		}
		
		
		player1[pos_y1] |= (1<<pos_x1);
		player1[pos_y1-1] |= (1<<pos_x1) | (1<<pos_x1)<<1 | (1<<pos_x1)>>1;
		player1[pos_y1-2] |= (1<<pos_x1) | (1<<pos_x1)<<2 | (1<<pos_x1)>>2;	
		
		ADMUX = 0b01100110;
		
		ADCSRA |= (1<<ADSC);
		while( ADCSRA & (1<<ADSC) )
		;
		
		result = ADCL;
		result = (result>>6) | (ADCH<<2);
		
		volt = result*5.0/1024;
		
		if (volt>4.5) {
		
		if(pos_x2<12) pos_x2++;
		
		} else if (volt<0.5) {
		
		
		if(pos_x2>3) pos_x2--;
		}
		
		
		
		ADMUX = 0b01100111;
		
		ADCSRA |= (1<<ADSC);
		while( ADCSRA & (1<<ADSC) )
		;
		
		result = ADCL;
		result = (result>>6) | (ADCH<<2);
		
		volt = result*5.0/1024;
		
		if (volt>4.5) {
		
		if(pos_y2>8) pos_y2--;
		
		} else if (volt<0.5) {
		
		if(pos_y2<13) pos_y2++;
		}
		
		
		player2[pos_y2] |= (1<<pos_x2);
		player2[pos_y2+1] |= (1<<pos_x2) | (1<<pos_x2)<<1 | (1<<pos_x2)>>1;
		player2[pos_y2+2] |= (1<<pos_x2) | (1<<pos_x2)<<2 | (1<<pos_x2)>>2;
		
		power1_pressed_time++;
		power2_pressed_time++;
		
		//player1 bullet
		
		if (flag1==1){
			if (power1_pressed_time<3){
				
				if(power1 >=2)
				{
					//bullet_count2=0;
					power1-=2;
					
					player1_powery[power1_count] = pos_y1;
					power1_count++;
				}
				
				
				
			} else {
				//player1 shoot
				bullet_x1[bullet_count1] = pos_x1;
				bullet_y1[bullet_count1] = pos_y1;
				bullet_count1++;
			}
			flag1=0;
			power1_pressed_time=0;
		}
		
		for (int a=0; a<bullet_count1; a++) {
			bullet_y1[a]++;
		}
		
		for (int a=0; a<bullet_count1; a++) {
			if ( bullet_y1[a]>15 ) {
				bullet_count1--;
				bullet_x1[a] = bullet_x1[bullet_count1];
				bullet_y1[a] = bullet_y1[bullet_count1];
				a--;
			}
			else
			{
				for(int b=0; b<bullet_count2; b++)
				{
					if(bullet_x1[a]==bullet_x2[b] && bullet_y1[a]==bullet_y2[b])
					{
						bullet_count1--;
						bullet_count2--;
						
						bullet_x1[a] = bullet_x1[bullet_count1];
						bullet_y1[a] = bullet_y1[bullet_count1];
						
						bullet_x2[b] = bullet_x2[bullet_count2];
						bullet_y2[b] = bullet_y2[bullet_count2];
						
						a--;
						b--;
					}
				}
				
				for(int c=0; c<power2_count;c++)
				{
					if(player2_powery[c]==bullet_y1[a])
					{
						bullet_count1--;
						
						bullet_x1[a] = bullet_x1[bullet_count1];
						bullet_y1[a] = bullet_y1[bullet_count1];
						
						a--;
					}
				}
			}
		}
		
		
		for (int a=0; a<bullet_count1; a++) {
			player1[bullet_y1[a]] |= 1<<bullet_x1[a];
		}
		
		
		//player 1 power effect
		
		for (int a=0; a<power1_count; a++) {
			player1_powery[a]++;
		}
		
		for (int a=0; a<power1_count; a++) {
			if ( player1_powery[a]>15 ) {
				power1_count--;
				player1_powerx[a] = player1_powerx[power1_count];
				player1_powery[a] = player1_powery[power1_count];
				a--;
			}
			
			else
			{
				for(int b=0; b<bullet_count2; b++)
				{
					if(player1_powery[a]==bullet_y2[b])
					{
						bullet_count2--;
						
						bullet_x2[b] = bullet_x2[bullet_count2];
						bullet_y2[b] = bullet_y2[bullet_count2];
					
						b--;
					}
				}
			}
		}
		
		for (int a=0; a<power1_count; a++) {
			player1[player1_powery[a]] |= 0b0111111111111110;
			player2[player1_powery[a]] |= 0b0111111111111110;
		}
		
		
		//player2 bullet
		
		if (flag2==1){
			if (power2_pressed_time<3) {
				//power
				if(power2>=2)
				{
					//bullet_count1=0;
					power2-=2;
					
					player2_powery[power2_count] = pos_y2;
					power2_count++;
				}
				
				
			} else {
				//player2 shoot
				bullet_x2[bullet_count2] = pos_x2;
				bullet_y2[bullet_count2] = pos_y2;
				bullet_count2++;
			}
			flag2=0;
			power2_pressed_time=0;
		}
		
		for (int a=0; a<bullet_count2; a++) {
			bullet_y2[a]--;
		}
		for (int a=0; a<bullet_count2; a++) {
			if ( bullet_y2[a]<0 ) {
				bullet_count2--;
				bullet_x2[a] = bullet_x2[bullet_count2];
				bullet_y2[a] = bullet_y2[bullet_count2];
				a--;
			}
			else
			{
				for(int b=0; b<bullet_count1; b++)
				{
					if(bullet_x1[b]==bullet_x2[a] && bullet_y1[b]==bullet_y2[a])
					{
						bullet_count1--;
						bullet_count2--;
						
						bullet_x1[b] = bullet_x1[bullet_count1];
						bullet_y1[b] = bullet_y1[bullet_count1];
						
						bullet_x2[a] = bullet_x2[bullet_count2];
						bullet_y2[a] = bullet_y2[bullet_count2];
						
						a--;
						b--;
					}
				}
				
				for(int c=0; c<power1_count;c++)
				{
					if(player1_powery[c]==bullet_y2[a])
					{
						bullet_count2--;
						
						bullet_x2[a] = bullet_x2[bullet_count2];
						bullet_y2[a] = bullet_y2[bullet_count2];
						
						a--;
					}
				}
			}
		}
	
		for (int a=0; a<bullet_count2; a++) {
			player2[bullet_y2[a]] |= 1<<bullet_x2[a];
		}
		
		
		//player 2 power effect
		
		for (int a=0; a<power2_count; a++) {
			player2_powery[a]--;
		}
		
		for (int a=0; a<power2_count; a++) {
			if ( player2_powery[a]<0 ) {
				power2_count--;
				player2_powerx[a] = player2_powerx[power2_count];
				player2_powery[a] = player2_powery[power2_count];
				a--;
			}
			
			else///////////////////////////////////
			{
				for(int b=0; b<bullet_count1; b++)
				{
					if(player2_powery[a]==bullet_y1[b])
					{
						bullet_count1--;
						
						bullet_x1[b] = bullet_x1[bullet_count1];
						bullet_y1[b] = bullet_y1[bullet_count1];
					
						b--;
					}
				}
			}
		}
		
		for (int a=0; a<power2_count; a++) {
			player1[player2_powery[a]] |= 0b0111111111111110;
			player2[player2_powery[a]] |= 0b0111111111111110;
		}
		
		
		
		//life of player 1
		for(int a=0; a<life1; a++)
		{
			player2[a] |= 1<<15;
		}
		
		//life of player 2
		for(int a=0; a<life2; a++)
		{
			player1[15-a] |= 1;
		}
		
		
		//power of player 1
		for(int a=0; a<power1; a++)
		{
			player2[a] |= 1<<1;
		}
		
		
		
		//power of player 2
		for(int a=0; a<power2; a++)
		{
			player1[15-a] |= 1<<14;
		}
		
		
		//condition check for shoot
		
		
		//player 1 is shooted
		for(int a=0; a<bullet_count2; a++)
		{
			temp1 = player1IsShooted(pos_x1,pos_y1,bullet_x2[a], bullet_y2[a]);
			if(temp1==1)
			{
				life1--;
				bullet_count2--;
				bullet_x2[a] = bullet_x2[bullet_count2];
				bullet_y2[a] = bullet_y2[bullet_count2];
				a--;
				
			}
		}
		
		if(life1<=0)
		{
			for(int a=0; a<500; a++){
				for (i=0; i<16; i++) {
					PORTC = (i<<4)>>4 | 0b11000000;
					unsigned short row = player2win[i];
					for (j=0; j<16; j++) {
						if ( (row & (1<<j))!=0 ) {
							PORTB = j;
							PORTC |= 0b01000000;
							PORTC &= 0b01111111;
						}
						else
						PORTC |= 0b11000000;
					}
				}
			}
			initializeVariables();
			continue;
		}
		
		
		//player 2 is shooted
		
		for(int a=0; a<bullet_count1; a++)
		{
			temp2 = player2IsShooted(pos_x2,pos_y2,bullet_x1[a], bullet_y1[a]);
			if(temp2==1)
			{
				life2--;
				bullet_count1--;
				bullet_x1[a] = bullet_x1[bullet_count1];
				bullet_y1[a] = bullet_y1[bullet_count1];
				a--;
				
			}
		}
		
		if(life2<=0)
		{
			for(int a=0; a<500; a++){
				for (i=0; i<16; i++) {
					PORTC = (i<<4)>>4 | 0b11000000;
					unsigned short row = player1win[i];
					for (j=0; j<16; j++) {
						if ( (row & (1<<j))!=0 ) {
							PORTA = j;
							PORTC |= 0b10000000;
							PORTC &= 0b10111111;
						}
						else
						PORTC |= 0b11000000;
					}
				}
			}
			initializeVariables();
			continue;
		}
		
		
		if(power_delay1 >= 30)
		{
			if(power1<6) power1++;
			power_delay1=0;
		}
		
		
		if(power_delay2 >= 30)
		{
			if(power2<6) power2++;
			power_delay2=0;
		}
		
		
		for(int a=0; a<3; a++){
			for (i=0; i<16; i++) {
				PORTC = (i<<4)>>4 | 0b11000000;
				unsigned short row = player1[i];
				for (j=0; j<16; j++) {
					if ( (row & (1<<j))!=0 ) {
						PORTA = j;
						PORTC |= 0b10000000;
						PORTC &= 0b10111111;
					}
					else
						PORTC |= 0b11000000;
				}
			}
			for (i=0; i<16; i++) {
				PORTC = (i<<4)>>4 | 0b11000000;
				unsigned short row = player2[i];
				for (j=0; j<16; j++) {
					if ( (row & (1<<j))!=0 ) {
						PORTB = j;
						PORTC |= 0b01000000;
						PORTC &= 0b01111111;
					}
					else
						PORTC |= 0b11000000;
				}
			}
		}
    }
}
