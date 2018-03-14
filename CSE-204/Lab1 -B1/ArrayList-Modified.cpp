#include<stdio.h>
#include<stdlib.h>

#define LIST_INIT_SIZE 2
#define NULL_VALUE -99999
#define SUCCESS_VALUE 99999

int listMaxSize;
int * list;
int length;


void initializeList()
{
	listMaxSize = LIST_INIT_SIZE;
	list = (int*)malloc(sizeof(int)*listMaxSize) ;
	length = 0 ;
}

int searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

int insertItem(int newitem)
{
	int * tempList ;
	if(listMaxSize==0) initializeList();
	else if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	}

	list[length] = newitem ; //store new item
	length++ ;
	return SUCCESS_VALUE ;
}
//Task 3
int shrink()
{
    int * tempList ;
	if (2*length == listMaxSize && listMaxSize!=LIST_INIT_SIZE)
	{
		//allocate new memory space for tempList
		listMaxSize = .5 * listMaxSize ;
		tempList = (int*) malloc (listMaxSize*sizeof(int)) ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        free(list) ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
        return SUCCESS_VALUE ;
	}
	return NULL_VALUE;
}

int deleteItemAt(int position) //version 2, do not preserve order of items
{
	if ( position >= length ) return NULL_VALUE;
	list[position] = list[length-1] ;
	length-- ;
	shrink();//This shrink serves for deleteItem (not anymore ->) and deleteLast as well
	return SUCCESS_VALUE ;
}


int deleteItem(int item) //version 1, preserve order of items
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return NULL_VALUE;
	/*int i=position;
	while(i<length-1){
        list[i]=list[i+1];
        i++;
	}
	length--;
	shrink();*/
	deleteItemAt(position) ;
	return SUCCESS_VALUE ;
}

void printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}
//Task 1
int getLength()
{
    return length;
}
//Task 2
int insertItemAt(int pos,int item)
{
    if(pos<0 || pos>=length) return NULL_VALUE;
    insertItem(list[pos]);
    list[pos]=item;
    return SUCCESS_VALUE;
}

//Task 4
int deleteLast()
{
    if(length>0)
    {
        //deleteItemAt(length-1);
        length--;
        shrink();
        return SUCCESS_VALUE;
    }
    return NULL_VALUE;
}
//Task 5
void Clear()
{
    free(list);
    length=0;
    listMaxSize=0;
}
//Task 6
void deleteAll()
{
    //while(length!=0) deleteLast();
    length=0;
    listMaxSize=LIST_INIT_SIZE;
    return;
}
int main(void)
{
    initializeList();
    while(1)
    {
        printf("1.  Insert new item.         2. Delete item at.       3. Delete item.\n");
        printf("4.  Print.                   5. Delete All Item.      6. Print Length. \n");
        printf("7.  Insert New Item At.      8. Delete Last Item.     9. Clear. \n");
        printf("10. exit.\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertItem(item);
        }
        else if(ch==2)
        {
            int pos;
            scanf("%d", &pos);
            deleteItemAt(pos);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            deleteItem(item);
        }
        else if(ch==4)
        {
            printList();
        }
        else if(ch==6)
        {
            printf("Current length = %d\n",getLength());
        }
        else if(ch==7)
        {
            int item,pos;
            scanf("%d%d", &pos,&item);
            insertItemAt(pos,item);
        }
        else if(ch==8)
        {
            deleteLast();
        }
        else if(ch==9)
        {
            Clear();
        }
        else if(ch==5)
        {
            deleteAll();
        }
        else if(ch==10)
        {
            Clear();
            break;
        }
    }

}
