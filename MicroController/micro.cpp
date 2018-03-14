/*
 * GccApplication1.cpp
 *
 * Created: 13-Apr-17 6:14:11 PM
 * Author : Nasif Zaman
 */
 
#include <avr/io.h>

struct Step{
	int num;
	Step* next;
	Step* next1;
	char color;
};

struct Token{
	Step* now;
	char color;
	int out;
};

struct Token Blue[4],Red[4],Yellow[4],Green[4];
struct Step B[18],R[18],Y[18],G[18];
struct Step *Last;

void tokenOut(Token* t){
	if(t->color=='B'){
		t->now=B;
	}
	else if(t->color=='R'){
		t->now=R;
	}
	else if(t->color=='Y'){
		t->now=Y;
	}
	else if(t->color=='G'){
		t->now=G;
	}
}

void thisStep(Token* t, int dice){
	if(t->out==1){
		int j=0;
		Step* s=t->now;
		for (j=0;j<dice;j++)
		{
			if((t->now)->color==t->color && ((t->now)->num-11)%18==0){
				t->now=(t->now)->next1;
			}
			else{
				t->now=(t->now)->next;
				if((t->now)->num==Last->num){
					if(j==dice-1) t->out=2;
					else{
						t->now=s;
						break;
					}
				}
			}
		}
		PORTC=(t->now)->num;
	}
	else if(t->out==0 && dice==6){
		t->out=1;
		tokenOut(t);
		PORTC=(t->now)->num;
	}
	return;
}

void initialBoard(){
	Last->num=72;
	Last->next=Last;//Need to change this in case of particular dice value to pass
	int i=0;
	for (i=0;i<4;i++)
	{
		Blue[i].color='B';
		Red[i].color='R';
		Yellow[i].color='Y';
		Green[i].color='G';
		Blue[i].out=0;
		Red[i].out=0;
		Yellow[i].out=0;
		Green[i].out=0;
	}
	for (i=0;i<18;i++)
	{
		B[i].num=i;
		R[i].num=i+18;
		Y[i].num=i+18*2;
		G[i].num=i+18*3;

		B[i].color='B';
		R[i].color='R';
		Y[i].color='Y';
		G[i].color='G';

		if(i==11){
			B[i].next=B+i+1;
			R[i].next=R+i+1;
			Y[i].next=Y+i+1;
			G[i].next=G+i+1;

			B[i].next1=R+i+2;
			R[i].next1=Y+i+2;
			Y[i].next1=G+i+2;
			G[i].next1=B+i+2;
		}
		else if(i==12){
			B[i].next=R;
			R[i].next=Y;
			Y[i].next=G;
			G[i].next=B;
		}
		else if(i==17){
			B[i].next=Last;
			R[i].next=Last;
			Y[i].next=Last;
			G[i].next=Last;
		}
		else{
			B[i].next=B+i+1;
			R[i].next=R+i+1;
			Y[i].next=Y+i+1;
			G[i].next=G+i+1;
		}
	}
	return;
}

int main(void)
{
    /* Replace with your application code */
	initialBoard();
	DDRA=0x00;
	DDRB=0x00;
	DDRC=0xFF;
	int dice,turn=0,player;
    while (1)
    {
		if(turn==0){
			player=PINB;
			dice=PINA;
			thisStep(Blue+player,dice);
		}
		else if(turn==1){
			player=PINB;
			dice=PINA;
			thisStep(Blue+player,dice);
		}
		else if(turn==2){
			player=PINB;
			dice=PINA;
			thisStep(Blue+player,dice);
		}
		else if(turn==3){
			player=PINB;
			dice=PINA;
			thisStep(Blue+player,dice);
		}
		turn=(turn+1)%4;
    }
	return 0;
}

