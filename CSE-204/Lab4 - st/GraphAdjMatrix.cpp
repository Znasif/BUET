#include<stdio.h>
#include<stdlib.h>
#define NULL_VALUE -999999
#define INFINITY 999999


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


//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	int ** matrix ; //adjacency matrix to store the graph
	//define other variables required for bfs such as color, parent, and dist
	int *parent,*dist;
	char *color;
	//you must use pointers and dynamic allocation

public:
    Queue q;
	Graph(bool dir = false);//default dir == false
	~Graph();
	void resetBfs();
	void setnVertices(int n);
	void addEdge(int u, int v);
	void removeEdge(int u, int v);
	bool isEdge(int u, int v);
    int getDegree(int u);
    void printAdjVertices(int u);
    bool hasCommonAdjacent(int u, int v);
    int getDist(int u, int v);
    void printGraph();
	void bfs(int source); //will run bfs in the graph
};


Graph::Graph(bool dir)
{
	nVertices = 0;
	nEdges = 0 ;
	matrix = 0 ;
	directed = dir ; //set direction of the graph
	//define other variables to be initialized
	parent = 0;
	color = 0;
	dist = 0;
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;

	//allocate space for the matrix
    matrix = new int*[nVertices];
    for(int i=0;i<nVertices;i++)
    {
        matrix[i] = new int[nVertices];
        for(int j=0;j<nVertices;j++)
            matrix[i][j] = 0; //initialize the matrix cells to 0
    }
}

void Graph::addEdge(int u, int v)
{
    //write your code here
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return;
    matrix[u][v] = 1;
    if(!directed) matrix[v][u] = 1;
    nEdges++;
}

bool Graph::isEdge(int u, int v)
{
    //returns true if (u,v) is an edge, otherwise should return false
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    if(matrix[u][v]!=0){
        return true;
    }
    return false;
}

void Graph::removeEdge(int u, int v)
{
    //write this function
    if(isEdge(u,v)){
        matrix[u][v] = 0;
        nEdges--;
        if(!directed) matrix[v][u] = 0;
    }
    return;
}

int Graph::getDegree(int u)
{
    //returns the degree of vertex u
    if(u<0 || u>=nVertices) return -1;
    int outDegree=0;
    if(directed==false){
        for(int j=0; j<nVertices;j++)
        {
            if(matrix[u][j]!=0)
                outDegree++;
        }
        return outDegree;
    }
    int inDegree=0;
    for(int j=0; j<nVertices;j++)
        {
            if(matrix[u][j]!=0)
                outDegree++;
            if(matrix[j][u]!=0){
                inDegree++;
            }
        }
    return inDegree+outDegree;
}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    if(u<0 || u>=nVertices) return;
    int k=0;
    printf("Adjacent vertices: ");
    for(int i=0;i<nVertices;i++){
        if(matrix[u][i]!=0){
            if(k!=0) printf(",");
            printf("%d",i);
            k++;
        }
    }
    printf("\n");
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    if(u<0 || u>=nVertices || v<0 || v>=nVertices) return false;
    for(int i=0;i<nVertices;i++){
        if(matrix[u][i] && matrix[v][i]) return true;
    }
    return false;
}

void Graph::bfs(int source)
{
    //write this function
    resetBfs();
    q.enqueue(source);
    dist[source]=0;
    color[source]='g';
    while(!q.empty()){
        int u=q.dequeue();
        for(int v=0;v<nVertices;v++){
            if(matrix[u][v]){
                if(color[v]=='w'){
                    color[v]='g';
                    parent[v]=u;
                    dist[v]=dist[u]+1;
                    q.enqueue(v);
                }
            }
        }
        color[u]='b';
    }
    //for(int i=0;i<nVertices;i++) printf("%d ",dist[i]);
}

void Graph::resetBfs(){
    delete[] parent;
    delete[] color;
    delete[] dist;
    parent = new int[nVertices];
    color = new char[nVertices];
    dist = new int[nVertices];
    for(int i=0;i<nVertices;i++)
    {
        parent[i]=-1;
        color[i]='w';
        dist[i]=INFINITY;
    }
}


int Graph::getDist(int u, int v)
{
    //returns the shortest path distance from u to v
    //must call bfs using u as the source vertex, then use distance array to find the distance
    bfs(u);
    return dist[v];
}


void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d:", i);
        for(int j=0; j<nVertices;j++)
        {
            if(matrix[i][j]!=0)
                printf(" %d", j);
        }
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(color)  delete[]color;
    if(parent) delete[]parent;
    if(dist)  delete[]dist;
    if(matrix){
        for(int i=0;i<nVertices;i++)delete[]matrix[i];
    }
    color=0,dist=0,parent=0,matrix=0;
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
    printf("\nEnter number of vertices: ");
    scanf("%d", &n);
    printf("\n");
    Graph g(f);
    g.setnVertices(n);

    while(1)
    {
        printf("1. Add edge  2. removeEdge\t  3. isEdge\n4. getDegree 5. printAdjVertices  6. printGraph\n7. getDist   8. hasCommonAdjacent 9. exit\n");

        int ch;
        printf("\nSelect any: ");
        scanf("%d",&ch);
        if(ch==1)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u, &v);
            g.addEdge(u, v);
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
        else if(ch==7)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u,&v);
            printf("Distance from %d to %d is: %d",u,v,g.getDist(u,v));
        }
        else if(ch==8)
        {
            int u,v;
            printf("Enter vertices: ");
            scanf("%d%d", &u,&v);
            if(g.hasCommonAdjacent(u,v)) printf("%d and %d have common vertices", u,v);
            else printf("They have no common vertices");
        }
        else if(ch==9)
        {
            break;
        }
        printf("\n");

    }

}
