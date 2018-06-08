#include<bits/stdc++.h>

using namespace std;


#define ms(x,val) memset(x,val,sizeof(x))
#define scan(x) scanf("%d",&x)
#define scanL(x) scanf("%I64d",&x)
#define print(x) printf("%d\n",x)
#define debug(x) printf("DEBUG: %d\n",x)
#define printL(x) printf("%I64d\n",x)
#define ull unsigned long long
#define iii long long
#define pi acos(-1)
#define pb push_back
#define PII pair<int,int>
#define vi vector<int>
#define itr_all(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)
#define MAPL map<long long, int >
#define MAPI map<int,int>
#define MAPP map< pair<int,int> , int>
#define MP make_pair
#define eps 1e-9
#define inf 999999999
#define MAXN 1000009
#define MOD 1000000007 // 10^9 + 7

// template < class T > T print( T a ) { cout<<(a)<<endl; return a;}
template < class T > T gcd(T a , T b ) { if(b==0) return a; else return gcd(b, a%b);}
template < class T > T lcm(T a , T b ) { return  a*b / gcd(a, b);}
template < class T > T absolute(T a ) { if(a>0) return a; else return -a;}
inline iii power(iii base,iii p) { iii ans=1; while(p>0) ans*=base,p-=1; return ans;}


struct Tree{
	int x;
	Tree *parent, *left, *right;

	Tree(int val){
		x = val;
		parent = left = right = NULL;
	}

	Tree* leftRotate(Tree *node){
		Tree *r;
		if(node->right == NULL) return node;
		r = node->right;
		node->right = r->left;
		r->left = node;
		return r;
	}

	Tree* rightRotate(Tree *node){
		Tree *l;
		if(node->left == NULL) return node;
		l = node->left;
		node->left = l->right;
		l->right = node;
		return l;
	}


	Tree* splay(Tree *node, int val){
		if( node == NULL || node->x == val) return node;

		if(val > node->x){
			node->right = splay(node->right, val);

			node = leftRotate(node);

		}
		else{
			node->left = splay(node->left, val);

			node = rightRotate(node);
		}

		return node;
	}

	Tree* splitNinsert(Tree *node, Tree *ins){
		if(node == NULL) return ins;
		Tree *l, *r;
		if(ins->x > node->x){

			l = node;
			r = node->right;
			node->right = NULL;
		}
		else{
			l = node->left;
			r = node;
			node->left = NULL;
		}

		ins->left = l;
		ins->right = r;
		return ins;
	}

	Tree* insert(Tree *node, int val){
		Tree *temp = NULL ;
		if(node != NULL) temp = splay(node, val);

		// cout<<"in insert:"<<endl;
		// printTreePreOrder(temp);
		// cout<<"ended"<<endl;
		Tree *ins;
		ins = new Tree(val);
		return splitNinsert(temp,ins);
	}


	Tree* join(Tree *l, Tree *r){
		l->right = r;
		return l;
	}

	Tree* del(Tree *node, int val){
		if(node == NULL) return node;
		node = splay(node,val);
		if(node->x != val) return node;

		if(node->left == NULL){
			Tree *ret = node->right;
			delete(node);
			return ret;
		}
		Tree *l,*r;
		l = splay(node->left,val);
		r = node->right;
		delete(node);
		return join(l,r);
	}

	Tree* find(Tree *node, int val){
		node = splay(node, val);
		if(node->x == val){
			cout<<"Value found..."<<endl;
		}
		else{
			cout<<"Value not found..."<<endl;
		}
		return node;


	}


	void printTreePreOrder(Tree *cur){
		if(cur == NULL) return;
		cout<< cur->x<<' ';

		printTreePreOrder(cur->left);
		printTreePreOrder(cur->right);

	}

	void printTreeInOrder(Tree *cur){
		if(cur == NULL) return;
		printTreeInOrder(cur->left);
		cout<< cur->x <<' ';
		printTreeInOrder(cur->right);
	}
};






int main(){

	Tree *root = NULL;
	int command,val;
	while(true){
		cin>>command;
		if(command == 0){
			cin>>val;
			root = root->insert(root, val);
		}
		else if(command == 1){
			cin>>val;
			root = root->del(root,val);
		}
		else if(command == 2){
			cin>>val;
			root = root->find(root,val);
		}


		// root->printTreePreOrder(root);
		root->printTreeInOrder(root);
		cout<<endl;
	}


	return 0;
}