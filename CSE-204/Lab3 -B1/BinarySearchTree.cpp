#include<stdio.h>
#include<stdlib.h>

#define FALSE_VALUE 0
#define TRUE_VALUE 1

struct treeNode
{
    int item;
    struct treeNode * left; //points to left child
    struct treeNode * right; //points to right child
};

struct treeNode * root;


void initializeTree()
{
    root = 0;
}


struct treeNode * searchItem(struct treeNode * node, int item)
{
    if(node==0) return 0;
    if(node->item==item) return node; //found, return node
    struct treeNode * t = 0;
    if(item < node->item)
        t = searchItem(node->left, item); //search in the left sub-tree
    else
        t = searchItem(node->right, item); //search in the right sub-tree
    return t;
};


struct treeNode * makeTreeNode(int item)
{
    struct treeNode * node ;
    node = (struct treeNode *)malloc(sizeof(struct treeNode));
    node->item = item;
    node->left = 0;
    node->right = 0;
    return node;
};

struct treeNode * insertItem(struct treeNode * node, int item)
{
    if(node==0) //insert as the root as the tree is empty
    {
        struct treeNode * newNode = makeTreeNode(item);
        root = newNode;
        return newNode;
    }

    if(node->item==item) return 0; //already an item exists, so return NULL

    if(item<node->item && node->left==0) //insert as the left child
    {
        struct treeNode * newNode = makeTreeNode(item);
        node->left = newNode;
        return newNode;
    }

    if(item>node->item && node->right==0) //insert as the right child
    {
        struct treeNode * newNode = makeTreeNode(item);
        node->right = newNode;
        return newNode;
    }

    if(item<node->item)
        return insertItem(node->left, item); //insert at left sub-tree
    else
        return insertItem(node->right, item); //insert at right sub-tree
}



int calcNodeHeight(struct treeNode * node) //return height of a node
{
    if(node==0) return -1;
    int l, r;
    l = calcNodeHeight(node->left);
    r = calcNodeHeight(node->right);
    if(l>r) return l+1;
    else return r+1;
}


int calcHeight(int item) //return height of an item in the tree
{
    struct treeNode * node = 0;
    node = searchItem(root, item);
    if(node==0) return -1; //not found
    else return calcNodeHeight(node);
}

int getSize(struct treeNode * node)
{
    if(node==0) return 0;
    return 1+getSize(node->left)+getSize(node->right);
}

int calcNodeDepth(struct treeNode * node) //return depth of a node
{
    int d=0,check=node->item;
    struct treeNode *go=root;
    while(go->item!=check){
        if(go->item<check) go=go->right;
        else go=go->left;
        d++;
    }
    if(node==go) return d;
    return -1;
}


int calcDepth(int item)//return depth of an item in the tree
{
    struct treeNode * node = 0;
    node = searchItem(root, item);
    if(node==0) return -1; //not found
    else return calcNodeDepth(node);
}

int deleteItem(struct  treeNode* node,int item)
{
    if(node==0)return 0;
    struct  treeNode * t,*prev;
    prev=0;
    t=root;

    while(t!=0)
    {
        if(item==t->item)break;
        prev=t;
        if(item<t->item)t=t->left;
        else t=t->right;

    }
    if(t==0)return 0;
    if(t->left==0&&t->right==0)
    {
        if(t==prev->left)prev->left=0;
        else prev->right=0;
        free(t);
    }
    else if(t->left==0)
    {
        if(t==prev->left)prev->left=t->right;
        else prev->right=t->right;
        free(t);
    }
    else if(t->right==0)
    {
        if(t==prev->left)prev->left=t->left;
        else  prev->right=t->left;
        free(t);
    }
    else
    {
        struct treeNode* temp;
        prev=t;
        temp=t->right;
        while(temp->left!=0){
            prev=temp;
            temp=temp->left;
        }
        t->item=temp->item;
        if(t->right==temp)t->right=temp->right;
        else prev->left=temp->right;
        free(temp);

    }


    //write your code here, return TRUE, FALSE

}
int rangeSearch(struct  treeNode * node, int leftBound, int rightBound) //returns number of items in the
{
    //write your codes here
    if(node==0)return 0;
    if(node->item>=leftBound&&node->item<=rightBound)
    {
        return 1+rangeSearch(node->left,leftBound,rightBound)+rangeSearch(node->right,leftBound,rightBound);
    }
    else if(node->item<leftBound)
    {
        return rangeSearch(node->right,leftBound,rightBound);
    }
    else if(node->item>rightBound)return rangeSearch(node->left,leftBound,rightBound);
}

int getMinItem() //returns the minimum item in the tree
{
    struct treeNode *node=root;
    while(node->left){
        node=node->left;
    }
    return node->item;
}



int getMaxItem() //returns the maximum item in the tree
{
    struct treeNode *node=root;
    while(node->right){
        node=node->right;
    }
    return node->item;
}

//int rangeSearch(struct treeNode * node, int leftBound, int rightBound) //returns number of items in the
//{
//    //write your codes here
//}
void printInOrder(struct treeNode * node, int height)
{
    if(node==0) return ;

    //print left sub-tree
    printInOrder(node->left, height-1);

    //print item
    for(int i=0;i<height;i++)printf("   ");
    printf("%03d\n",node->item);

    //print right sub-tree
    printInOrder(node->right, height-1);
}

int go(struct treeNode *node){
    if(!node) return 0;
    return node->item+go(node->left)+go(node->right);
}

int subtreeSum(int item){
    struct treeNode *node;
    node=searchItem(root,item);
    if(!node) return -1;
    return go(node);

}


int main(void)
{
    initializeTree();
    while(1)
    {
        printf("1. Insert item. 2. Delete item. 3. Search item. \n");
        printf("4. Print height of tree. 5. Print height of an item. \n");
        printf("6. PrintInOrder. 7. Minterm. 8. MaxTerm 9. getSize 10. calcDepth \n11. SubtreeSum 12. rangeSearch 13. exit\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            int item;
            scanf("%d", &item);
            insertItem(root, item);
        }
        else if(ch==2)
        {
            int item;
            scanf("%d", &item);
            deleteItem(root, item);
        }
        else if(ch==3)
        {
            int item;
            scanf("%d", &item);
            struct treeNode * res = searchItem(root, item);
            if(res!=0) printf("Found.\n");
            else printf("Not found.\n");
        }
        else if(ch==4)
        {
            int height = calcNodeHeight(root);
            printf("Height of tree = %d\n", height);
        }
        else if(ch==5)
        {
            int item;
            scanf("%d", &item);
            int height = calcHeight(item);
            printf("Height of %d = %d\n", item, height);
        }
        else if(ch==6)
        {
            int h = calcNodeHeight(root);
            printf("\n--------------------------------\n");
            printInOrder(root, h);
            printf("--------------------------------\n");
        }
        else if(ch==7)
        {
            printf("%d\n",getMinItem());
        }
        else if(ch==8)
        {
            printf("%d\n",getMaxItem());
        }
        else if(ch==9)
        {
            printf("%d\n",getSize(root));
        }
        else if(ch==10)
        {
            int c;
            scanf("%d", &c);
            printf("%d\n",calcDepth(c));
        }
        else if(ch==11){
            int c;
            scanf("%d", &c);
            printf("%d\n", subtreeSum(c));
        }
        else if(ch==12){
            int a,b;
            scanf("%d%d", &a,&b);
            printf("%d",rangeSearch(root,a,b));
        }
        else if(ch==13)
        {
            break;
        }
    }

}
