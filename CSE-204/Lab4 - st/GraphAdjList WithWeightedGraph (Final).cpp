#include<stdio.h>
#include<stdlib.h>
#define NULL_VALUE -999999
#define INFINITY 999999
#define WHITE 1
#define GREY 2
#define BLACK 3

class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue();
    ~Queue();
    void enqueue(int item); //insert item in the queue
    int dequeue(); //returns the item according to FIFO
    bool empty(); //return true if Queue is empty
};

Queue::Queue()
{
    queueInitSize = 2 ;
    queueMaxSize = queueInitSize;
    data = new int[queueMaxSize] ; //allocate initial memory
    length = 0 ;
    front = 0;
    rear = 0;
}


void Queue::enqueue(int item)
{
	if (length == queueMaxSize)
	{
		int * tempData ;
		//allocate new memory space for tempList
		queueMaxSize = 2 * queueMaxSize ;
		tempData = new int[queueMaxSize] ;
		int i, j;
		j = 0;
		for( i = rear; i < length ; i++ )
		{
			tempData[j++] = data[i] ; //copy items from rear
		}
		for( i = 0; i < rear ; i++ )
		{
			tempData[j++] = data[i] ; //copy items before rear
		}
		rear = 0 ;
		front = length ;
		delete[] data ; //free the memory allocated before
		data = tempData ; //make list to point to new memory
	}

	data[front] = item ; //store new item
	front = (front + 1) % queueMaxSize ;
	length++ ;
}


bool Queue::empty()
{
	if(length == 0) return true ;
	else return false ;
}


int Queue::dequeue()
{
	if(length == 0) return NULL_VALUE ;
	int item = data[rear] ;
	rear = (rear + 1) % queueMaxSize ;
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; //deallocate memory
    data = 0; //set to NULL
}

//****************Queue class ends here************************

//****************Dynamic ArrayList class based************************
class ArrayList
{
	int * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(int item) ;
    void insertItem(int item) ;
	void removeItem(int item) ;
	void removeItemAt(int item);
	int getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new int[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(int newitem)
{
	int * tempList ;
	if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = new int[listMaxSize] ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        delete[] list ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};

	list[length] = newitem ; //store new item
	length++ ;
}

int ArrayList::searchItem(int item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

void ArrayList::removeItemAt(int position) //do not preserve order of items
{
	if ( position < 0 || position >= length ) return ; //nothing to remove
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(int item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; //nothing to remove
	removeItemAt(position) ;
}


int ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return NULL_VALUE ;
	return list[position] ;
}

int ArrayList::getLength()
{
	return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************

//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList ;
	ArrayList * weightedList;
	//define other variables required for bfs such as color, parent, and dist
	//you must use pointers and dynamic allocation

public:
	Graph(bool dir = false);
	~Graph();
	void setnVertices(int n);
	void addEdge(int u, int v, int w);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getDegree(int u);
    void printAdjVertices(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	weightedList =0;
	directed = dir ; //set direction of the graph
	//define other variables to be initialized
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	if(adjList!=0){
        delete[] adjList ; //delete previous list
        delete[] weightedList;
	}
	adjList = new ArrayList[nVertices] ;
	weightedList = new ArrayList[nVertices];
}

void Graph::addEdge(int u, int v,int w)
{
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
    this->nEdges++ ;
	adjList[u].insertItem(v) ;
	weightedList[u].insertItem(w);
	if(!directed){
        adjList[v].insertItem(u) ;
        weightedList[v].insertItem(w);
	}
}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(!(isEdge(u,v))) return;
    this->nEdges--;
    int position = adjList[u].searchItem(v);
    adjList[u].removeItemAt(position);
	weightedList[u].removeItemAt(position);
	if(!directed){
        position = adjList[v].searchItem(u);
        adjList[v].removeItemAt(position);
        weightedList[v].removeItemAt(position);
	}
	return;
}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    if(adjList[u].searchItem(v)!=NULL_VALUE && adjList[v].searchItem(u)!=NULL_VALUE){
        return true;
    }
    return false;
}

int Graph::getDegree(int u)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return -1;
    int outDegree=0,inDegree=0;
    outDegree=adjList[u].getLength();
    if(directed==true){
        for(int v=0;v<nVertices;v++){
            if(adjList[v].searchItem(u)!=NULL_VALUE) inDegree++;
        }
    }
    return inDegree+outDegree;
}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u<0 || u>=nVertices) return;
    int l=adjList[u].getLength();
    printf("Adjacent vertices of %d: ",u);
    int k=0;
    for(int i=0;i<l;i++){
        if(k!=0) printf(",");
        printf("%d(%d)", adjList[u].getItem(i),weightedList[u].getItem(i));
        k++;
    }
    printf("\n");
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    int l=adjList[u].getLength();
    for(int i=0;i<l;i++){
        if(adjList[v].searchItem(adjList[u].getItem(i))) return true;
    }
    return false;
}



void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printAdjVertices(i);
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(adjList) delete[]adjList;
    if(weightedList) delete[]weightedList;
    adjList=0,weightedList=0;
}


//**********************Graph class ends here******************************


//******main function to test your code*************************
int main(void)
{
    int n,c;
    bool f;
    printf("To create a non-directed graph enter 1 else enter 0: ");
    scanf("%d",&c);
    if(c){
        f=false;
    }
    else f=true;
    Graph g(f);
    printf("To create a non-weighted graph enter 1 else enter 0: ");
    scanf("%d",&c);
    printf("\nEnter number of vertices: ");
    scanf("%d", &n);
    printf("\n");
    g.setnVertices(n);

    while(1)
    {
        printf("1. Add edge  2. removeEdge\t  3. isEdge\n4. getDegree 5. printAdjVertices  6. printGraph\n7. exit      8. hasCommonAdjacent\n");

        int ch;
        printf("\nSelect any: ");
        scanf("%d",&ch);
        if(ch==1)
        {
            int u,v,w=c;
            printf("Enter vertices: ");
            scanf("%d%d", &u, &v);
            if(c==0){
                printf("Enter \"weight\":");
                scanf("%d",&w);
            }
            g.addEdge(u, v, w);
        }
        else if(ch==2)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u,&v);
            g.removeEdge(u,v);
        }
        else if(ch==3)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u,&v);
            if(g.isEdge(u,v)){
                printf("There is an edge between vertex %d & vertex %d\n", u,v);
            }
            else{
                printf("%d and %d aren't connected\n", u,v);
            }
        }
        else if(ch==4)
        {
            int u;
            printf("Enter vertex: ");
            scanf("%d", &u);
            printf("Number of adjacent vertices: %d\n",g.getDegree(u));
        }
        else if(ch==5)
        {
            int u;
            printf("Enter vertex: ");
            scanf("%d", &u);
            g.printAdjVertices(u);
        }
        else if(ch==6)
        {
            g.printGraph();
        }
        else if(ch==8)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u,&v);
            if(g.hasCommonAdjacent(u,v)) printf("%d and %d have common vertices", u,v);
            else printf("They have no common vertices");
        }
        else if(ch==7)
        {
            break;
        }
        printf("\n");

    }

}
