#include<stdio.h>
#include<stdlib.h>


#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

struct listNode
{
    int item;
    struct listNode * next;
    struct listNode * prev;
};

struct listNode * list;

struct listNode * tail;

void initializeList()
{
    list = 0;  //initially set to NULL
    tail = 0;
}

int insertFirst(int item) //insert at the beginning
{
	struct listNode * newNode ;
	newNode = (struct listNode*) malloc (sizeof(struct listNode)) ;
	newNode->item = item ;

	if(list==0) //inserting the first item
    {
        newNode->next = 0;
        newNode->prev = 0;
        list = newNode;
        tail = newNode;
    }
    else
    {
        newNode->next = list;
        list->prev = newNode;
        newNode->prev = 0;
        list = newNode;
    }
	return SUCCESS_VALUE ;
}


int insertLast(int item)
{
    struct listNode *newNode;
    newNode = (struct listNode*) malloc (sizeof(struct listNode)) ;
	newNode->item = item ;
	if(list==0){
        return insertFirst(item);
	}
	tail->next=newNode;
	newNode->next=0;
	newNode->prev=tail;
	tail=newNode;
	return SUCCESS_VALUE;
    //write your codes here
}

int deleteLast()
{
    int item;
    if(tail==0){
        return NULL_VALUE;
    }
    else if(tail==list){
        list=0;
        item=tail->item;
        free(tail);
        tail=list;
        return item;
    }
    listNode *keep;
    item=tail->item;
    tail->prev->next=0;
    keep=tail->prev;
    free(tail);
    tail=keep;
    return item;
}


struct listNode * searchItem(int item)
{
	struct listNode * temp ;
	temp = list ; //start at the beginning
	while (temp != 0)
	{
		if (temp->item == item) return temp ;
		temp = temp->next ; //move to next node
	}
	return 0 ; //0 means invalid pointer in C, also called NULL value in C
}

void printListForward()
{
    struct listNode * temp;
    temp = list;
    while(temp!=0)
    {
        printf("%d->", temp->item);
        temp = temp->next;
    }
    printf("\n");
}

void printListBackward()
{
	//will print the list backward starting from the tail position
	struct listNode * temp;
    temp = tail;
    while(temp!=0)
    {
        printf("%d->", temp->item);
        temp = temp->prev;
    }
    printf("\n");
}

int deleteAfter(int item){
    if(item==NULL_VALUE){
        if(list==0){
            return NULL_VALUE;
        }
        else if(tail==list){
            list=0;
            free(tail);
            tail=list;
            return SUCCESS_VALUE;
        }
        listNode *keep;
        list->next->prev=0;
        keep=list->next;
        free(list);
        list=keep;
        return SUCCESS_VALUE;

    }
    struct listNode * temp ;
	temp = list ; //start at the beginning
	while (temp != 0)
	{
		if (temp->item == item) break ;
		temp = temp->next ; //move to next node
	}
	if(temp==0 || temp==tail){
        return NULL_VALUE;
	}
	if(tail!=list && temp==tail->prev){
        return deleteLast();
	}
	struct listNode *keep;
	keep=temp->next;
	temp->next->next->prev=temp;
	temp->next=temp->next->next;
	free(keep);
	//temp->prev->prev->next=temp;
	//temp->prev=temp->
	return SUCCESS_VALUE;
}
int main(void)
{
    initializeList();
    while(1)
    {
        printf("1. Insert new item. 2. Delete item. 3. Search item. \n");
        printf("4. Print forward. 5. Print backward. 6. exit. 7. InsertLast 8. DeleteAfter\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertFirst(item);
        }
        else if(ch==2)
        {
            int item = deleteLast();
            if(item!=NULL_VALUE) printf("Deleted: %d\n", item);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            struct listNode * res = searchItem(item);
            if(res!=0) printf("Found.\n");
            else printf("Not found.\n");
        }
        else if(ch==7)
        {
            int a;
            scanf("%d",&a);
            insertLast(a);
        }
        else if(ch==4)
        {
            printListForward();
        }
        else if(ch==5)
        {
            printListBackward();
        }
        else if(ch==6)
        {
            break;
        }
        else if(ch==8)
        {
            int a;
            scanf("%d", &a);
            deleteAfter(a);
        }
    }

}
