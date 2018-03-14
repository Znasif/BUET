/*
 * GccApplication1.cpp
 *
 * Created: 13-Apr-17 6:14:11 PM
 * Author : Nasif Zaman
 */

#include <avr/io.h>
#define F_CPU 1000000
#include<util/delay.h>
//Red is the computer, Green is player
#include<stdio.h>
#include<stdlib.h>

unsigned short gFrame[] = {
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

unsigned short rFrame[] = {
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
typedef struct Step Step;
typedef struct Token Token;

struct Step{
	int num;
	Step* next;
	Step* next1;
	char color;
	unsigned short x,y;
};

struct Token{
	Step* now;
	int num;
	char color;
	int out;
}Red[4],Green[4];

int mini(int x,int y)
{
    if(x<y)return x;
    return y;
}

int getColor(){
    int r;
    //printf("Select Dice : ");
    //scanf("%d", &r);
    r=rand()%6+1;
	return r;
}

int getPlayer(){
    int r;
    //printf("Select Token : ");
    //scanf("%d", &r);
    r=rand()%4;
	return r;
}
int blink=0;
void Print2Matrix(){
    Token* t;
    int i,j;

    for(i=0;i<4;i++){
        t=Red+i;
        rFrame[15-t->now->y] |= 1<<(15-t->now->x);
        t=Green+i;
        gFrame[15-t->now->y] |= 1<<(15-t->now->x);
    }

    for( i=0;i<16;i++){
        for(j=15;j>=0;j--){//printf("%d",mini(1,gFrame[i]&(1<<j))|mini(1,rFrame[i]&(1<<j)));
            if(mini(1,gFrame[i]&(1<<j))){
                PORTA=i;
                PORTB=j;
                _delay_us(300);
            }
            if(mini(1,rFrame[i]&(1<<j)) && blink==10){
                PORTA=i;
                PORTB=j;
                _delay_us(500);
            }
        }
        //printf("\n");
    }
    if(blink==10) blink=0;

}
int delay=0;
int getNum(Token* t){
    if(t->now==NULL) return -3;
    if(t->out==0) return -1;
    if(t->out==2) return -2;
    return t->now->num;
}
//struct Token Red[4],Green[4],Yellow[4],Blue[4];
struct Step B[18],R[18],Y[18],G[18];
struct Step g2R[4],g2G[4],r2R[4],r2G[4];
struct Step y2RR[4],y2R[4],y2G[4],b2G[4];
struct Step b2GG[4],b2R[4],resG[4],resR[4];
struct Step *Last;

void refreshFrame(){
	int i;
	for(i=0;i<16;i++){
        rFrame[i]=0b0000000000000000;
        gFrame[i]=0b0000000000000000;
	}
}

void tokenOut(struct Token* t){
	if(t->color=='B'){
		t->now=B;
	}
	else if(t->color=='R'){
		t->now=y2R+t->num;
	}
	else if(t->color=='Y'){
		t->now=Y;
	}
	else if(t->color=='G'){
		t->now=b2G+t->num;
	}
}

void tokenIn(Token* t){
	if(t->color=='B'){
		t->now=B;
	}
	else if(t->color=='R'){
		t->now=y2RR+t->num;
	}
	else if(t->color=='Y'){
		t->now=Y;
	}
	else if(t->color=='G'){
		t->now=b2GG+t->num;
	}
}

void replaceToken(Token* eaten, Token* t){
	t->now=eaten->now;
	tokenIn(eaten);
}



void thisStep(Token* t, int dice){
	if(dice==0) return;
	if(t->out==1){
        //printf("%d\n",t->now->num);
		int j=0;
		Step* s=t->now;
		for (j=0;j<dice;j++)
		{
            int flag=0;
		    if(t->now->color==t->color){

                if(t->color=='R' && t->now->num==48){
                    t->now=(t->now)->next1;
                    flag=1;
                }
                else if(t->color=='G' && t->now->num==23){
                    t->now=(t->now)->next1;
                    flag=1;
                }
			}
			if(flag==0){
				t->now=(t->now)->next;
				printf("HE");
				int p=getNum(t);
				printf("RE : %d\n",p);
				if(p>=100){
					if(j==dice-1){
                        if(t->color=='R') t->now = resR+(p-100);
                        if(t->color=='G') t->now = resG+(p-100);
                        t->out=2;
					}
					else{
						t->now=s;
					}
                    break;
				}
				else if(j==dice-1 && (p>0 && p<50) && !(p==12 || p==25 || p==37)){
                    int k;
                    for(k=0;k<4;k++){
                        if(t->color=='R' && p==getNum(Green+k)){
                            replaceToken(Green+k,t);
                            flag=1;
                            break;
                        }
                        else if(t->color=='R' && p==getNum(Red+k) && t->num!=Red[k].num){
                            t->now=s;
                            flag=1;
                            break;
                        }

                        if(t->color=='G' && p==getNum(Red+k)){
                            replaceToken(Red+k,t);
                            flag=1;
                            break;
                        }
                        else if(t->color=='G' && p==getNum(Green+k) && t->num!=Green[k].num){
                            t->now=s;
                            flag=1;
                            break;
                        }
                    }
                    if(flag==1) break;
				}
			}
		}
	}
	else if(t->out==0 && dice==6){
		t->out=1;
		//printf("Name");
		tokenOut(t);
	}
	return;
}

void initialBoard(){

	//Last->next=Last;//Need to change this in case of particular dice value to pass
	//printf("d");
	unsigned short i=0;
	for (i=0;i<4;i++)
	{
		//Blue[i].color='B';
		Red[i].color='R';
		//Yellow[i].color='Y';
		Green[i].color='G';
		//Blue[i].out=0;
		Red[i].out=0;
		//Yellow[i].out=0;
		Green[i].out=0;
		Red[i].now=y2RR+i;
		Green[i].now=b2GG+i;
		Red[i].num=i;
		Green[i].num=i;
	}
    //printf("d");
	for(i=0;i<12;i++){
		Y[i].num=i;
		G[i].num=i+12;
		B[i].num=i+25;
		R[i].num=i+37;
	}
	G[12].num=24;
	R[12].num=49;


	for (i=0;i<18;i++)
	{
		B[i].color='B';
		R[i].color='R';
		Y[i].color='Y';
		G[i].color='G';

		if(i==11){
			B[i].next=R;
			R[i].next=R+i+1;
			Y[i].next=G;
			G[i].next=G+i+1;

			//B[i].next1=R+i+2;
			R[i].next1=Y+i+2;
			//Y[i].next1=G+i+2;
			G[i].next1=B+i+2;
		}
		else if(i==12){
			//B[i].next=R;
			R[i].next=Y;
			//Y[i].next=G;
			G[i].next=B;
		}
		else if(i==17){
			B[i].next=resR;
			//R[i].next=Last;
			Y[i].next=resG;
			//G[i].next=Last;
		}
		else{
			B[i].next=B+i+1;
			R[i].next=R+i+1;
			Y[i].next=Y+i+1;
			G[i].next=G+i+1;
		}
	}


	for(i=0;i<6;i++) {Y[i].x=5;Y[i].y=1+i;}
	for(i=6;i<11;i++) {Y[i].x=10-i;Y[i].y=6;}
	for(i=0;i<6;i++) {B[i].x=8;B[i].y=14-i;}
	for(i=6;i<11;i++) {B[i].x=3+i;B[i].y=9;}
	for(i=0;i<4;i++) {R[i].x=12-i;R[i].y=7;}
	for(i=4;i<11;i++) {R[i].x=9;R[i].y=10-i;}
	for(i=0;i<4;i++) {G[i].x=1+i;G[i].y=8;}
	for(i=4;i<11;i++) {G[i].x=4;G[i].y=5+i;}
	for(i=0;i<5;i++) {Y[13+i].x=7;Y[13+i].y=i+1;Y[13+i].num=50;}
	for(i=0;i<5;i++) {B[13+i].x=6;B[13+i].y=14-i;B[13+i].num=50;}
	Y[11].x=0;Y[11].y=8;
	B[11].x=13;B[11].y=7;
	R[11].x=7;R[11].y=0;
	R[12].x=5;R[12].y=0;
	G[11].x=6;G[11].y=15;
	G[12].x=8;G[12].y=15;


	for(i=0;i<4;i++) {
		g2R[i].x=0;g2R[i].y=14-i;g2R[i].color='R';
		g2G[i].x=2;g2G[i].y=14-i;g2G[i].color='G';g2G[i].next=G+1;
		r2R[i].x=11;r2R[i].y=4-i;r2R[i].color='R';r2R[i].next=R+1;
		r2G[i].x=13;r2G[i].y=4-i;r2G[i].color='G';r2G[i].next=R+1;
		y2R[i].x=i;y2R[i].y=2;y2R[i].color='R';y2R[i].next=Y+1;
		y2RR[i].x=i;y2RR[i].y=0;y2RR[i].color='R';
		y2G[i].x=i;y2G[i].y=4;y2G[i].color='G';y2G[i].next=Y+1;
		b2G[i].x=13-i;b2G[i].y=13;b2G[i].color='G';b2G[i].next=B+1;
		b2GG[i].x=13-i;b2GG[i].y=15;b2GG[i].color='G';
		b2R[i].x=13-i;b2R[i].y=11;b2R[i].color='R';b2R[i].next=B+1;
		resG[i].x=15;resG[i].y=15-i;resG[i].color='G';
		resR[i].x=15;resR[i].y=3-i;resR[i].color='R';
		g2R[i].num=0;
		r2R[i].num=37;
		b2R[i].num=25;
		y2R[i].num=12;
		b2GG[i].num=-1;
		y2RR[i].num=-1;
		resG[i].num=100;
		resR[i].num=100;
	}
    //printf("d");
	return;
}

void selectPlayer(int x, int y, int z){
	int g[4],i;
	Token* t;

	if(x==6){
		for(i=0;i<4;i++){
			t=Red+i;
			if(t->out==0){ thisStep(t,x);x=0;break; }
		}
	}
	if(y==6){
		for(i=0;i<4;i++){
			t=Red+i;
			if(t->out==0){ thisStep(t,y);y=0;break; }
		}
	}
    //printf("%d %d %d\n",x,y,z);
	for(i=0;i<4;i++){
		t=Red+i;
		int p=getNum(t);
		if(p>=0){
			if(p+x+y+z>48){
				thisStep(Red+i,x);
				x=0;
				p=getNum(t);
				if(p<=48){
					thisStep(Red+i,y);
					y=0;
					p=getNum(t);
					if(p<=48){
						thisStep(Red+i,z);
						z=0;
						p=getNum(t);
					}
				}
			}
		}
	}
	//printf("%d %d %d\n",x,y,z);
	for(i=0;i<4;i++){
		t=Green+i;
		g[i]=getNum(t);
	}
	for(i=0;i<4;i++){
		t=Red+i;
		int min=100,idx=-1,j,p;
		for(j=0;j<4;j++){
			p=getNum(t);
			if(g[j]<p && g[j]!=-1){
				if(min>p-g[j]) min = p-g[j];
				idx=i;
			}
		}
		if(idx!=-1 && min<6){
			thisStep(Red+idx,x);
			x=0;min+=x;
		}
		if(idx!=-1 && min<6){
			thisStep(Red+idx,y);
			y=0;min+=y;
		}
		if(idx!=-1 && min<6){
			thisStep(Red+idx,z);
			z=0;min+=z;
		}
	}
	for(i=0;i<4;i++){
		t=Red+i;
		int j,p=getNum(t);
		for(j=0;j<4;j++){
			if(p+x+y+z==g[j]){ thisStep(t,x);thisStep(t,y);thisStep(t,z);x=y=z=0;}
			else if(p+x+y==g[j]){ thisStep(t,x);thisStep(t,y);x=y=0;}
			else if(p+y+z==g[j]){ thisStep(t,z);thisStep(t,y);y=z=0;}
			else if(p+x+z==g[j]){ thisStep(t,x);thisStep(t,z);x=z=0;}
			else if(p+x==g[j]){ thisStep(t,x);x=0;}
			else if(p+y==g[j]){ thisStep(t,y);y=0;}
			else if(p+z==g[j]){ thisStep(t,z);z=0;}
		}
	}
	//printf("%d %d %d\n",x,y,z);
	if(x+y+z!=0){

		for(i=0;i<4;i++){
			t=Red+i;
			int p=getNum(t);
			if(p+x<49) { thisStep(t,x);x=0; }
			if(p+y<49) { thisStep(t,y);y=0; }
			if(p+z<49) { thisStep(t,z);z=0; }
		}
	}
}

int main(void)
{
    /* Replace with your application code */
	initialBoard();
    //Green[0].now=G+5;
    //Green[0].out=1;
    //Red[0].now=G+6;
    //Red[0].out=1;
	DDRA=0xFF;
	DDRB=0xFF;
	DDRC=0xFF;
	int dice1,dice2,dice3,turn=0,player;
    while (1)
    {
        delay++;
        blink++;
        if(delay<30){
            Print2Matrix();
            continue;
        }
		if(turn==0){
			dice1=getColor();
			if(dice1==6){
				dice2=getColor();
				if(dice2==6){
					dice3=getColor();
					player=getPlayer();
					thisStep(Green+player,dice1);
					player=getPlayer();
					thisStep(Green+player,dice2);
					player=getPlayer();
					thisStep(Green+player,dice3);
				}
				else{
					player=getPlayer();
					thisStep(Green+player,dice1);
					player=getPlayer();
					thisStep(Green+player,dice2);
				}
			}else{
				player=getPlayer();
				thisStep(Green+player,dice1);
			}
		}
		else if(turn==1){
			dice1=getColor();
			if(dice1==6){
				dice2=getColor();
				if(dice2==6){
					dice3=getColor();
					selectPlayer(dice1,dice2,dice3);
				}
				else{
					selectPlayer(dice1,dice2,0);
				}
				}else{
				selectPlayer(dice1,0,0);
			}
		}
		turn=(turn+1)%2;
		Print2Matrix();
		refreshFrame();
        delay=0;
    }

    //Print2Matrix();

	return 0;
}
