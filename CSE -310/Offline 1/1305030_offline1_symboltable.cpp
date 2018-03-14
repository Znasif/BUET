#include<bits/stdc++.h>

#define read freopen("input.txt", "r", stdin)
#define write freopen("output.txt", "w", stdout)

using namespace std;

class SymbolInfo
{
private:
    string SymbolName;
    string SymbolType;
    SymbolInfo *next;
public:
    int checkEqual(string s){
        return s==SymbolName;
    }
    SymbolInfo(string x, string y){
        SymbolName=x;
        SymbolType=y;
        next=NULL;
    }
    string getSymbol(){
        return SymbolName;
    }
    string getType(){
        return SymbolType;
    }
    SymbolInfo* getNext(){
        return next;
    }
    void setNext(SymbolInfo* x){
        next=x;
    }
    ~SymbolInfo(){}
};

class Symbols{
private:
    SymbolInfo *head=0,*tail=NULL;
    int total,index;
public:
    Symbols(){total=0;};
    int getTotal(){ return total; }
    SymbolInfo* getHead(){
        return head;
    }
    SymbolInfo* getTail(){
        return tail;
    }
    void Insert(string x, string y,int index){
        SymbolInfo* S=new SymbolInfo(x,y);
        this->index=index;
        if(total==0){
            cout<<"I "<<x<<" "<<y<<endl;
            head=S;
            tail=S;
            total++;
            cout<<"<"<<S->getSymbol()<<","<<S->getType()<<"> inserted at position "<<index<<", "<<total-1<<endl;
            return ;
        }
        cout<<"I "<<x<<" "<<y<<endl;
        SymbolInfo* temp=head;
        int i=0;
        while(temp!=0){
            if(temp->checkEqual(x)){
                cout<<"<"<<temp->getSymbol()<<","<<temp->getType()<<"> already exists"<<endl;
                return;
            }
            temp=temp->getNext();
            i++;
        }
        total++;
        tail->setNext(S);
        tail=S;
        cout<<"<"<<S->getSymbol()<<","<<S->getType()<<"> inserted at position "<<index<<", "<<total-1<<endl;
    }
    void LookUp(string x){
        SymbolInfo* temp=head;
        cout<<"L "<<x<<endl;
        int i=0;
        while(temp!=0){
            if(temp->checkEqual(x)){
                cout<<"<"<<temp->getSymbol()<<","<<temp->getType()<<"> found at position "<<index<<", "<<i<<endl;
                return;
            }
            if(temp->getNext()!=0) temp=temp->getNext();
            i++;
        }
        cout<<x<<" not found"<<endl;
        return;
    }
    void Delete(string x){
        SymbolInfo *temp=head,*keep=0;
        cout<<"D "<<x<<endl;
        int i=0;
        while(temp!=0){
            if(temp->checkEqual(x)){
                if (temp==head)
                {
                    if(tail==head){
                        tail=0;
                        head=0;
                        cout<<"Deleted from "<<index<<", "<<0<<endl;
                        total--;
                        return;
                    }
                    head = head->getNext() ;
                    free(temp) ;
                }
                else if(temp==tail)
                {
                    keep->setNext(0);
                    free(tail);
                    tail=keep;
                }
                else
                {
                    keep->setNext(temp->getNext()) ;
                    free(temp);
                }
                total--;
                cout<<"Deleted from "<<index<<", "<<i<<endl;
                return;
                }
            i++;
            keep=temp;
            temp=temp->getNext();
        }
        cout<<x<<" not found"<<endl;
        return;
    }
};

class SymbolTable{
private:
    int total;
    Symbols* Infos;
public:
    SymbolTable(int tot){
        if(tot<1) tot=1;
        total=tot;
        Infos= new Symbols[tot];
    }
    void Print(){
        SymbolInfo* temp;
        cout<<"P"<<endl;
        for(int i=0; i<total; i++){
            if(Infos[i].getTotal()!=0){
                cout<<i<<" -->";
                temp=Infos[i].getHead();
                do{
                    cout<<" < "<<temp->getSymbol()<<" : "<<temp->getType()<<">";
                    temp=temp->getNext();
                }while(temp!=0);
                cout<<endl;
            }
            else{
                cout<<i<<" --> "<<endl;
            }
        }
    }
    int hashFunction(string x){
        //int r=x[0]%total;

        int r=0,q=1;
        for(int i=0; i<x.size(); i++){
            r+=(x[i]%10)*q;
            q=q*10;
        }

        return r%total;
    }
    void LookUp(string x){
        int r=hashFunction(x);
        Infos[r].LookUp(x);
    }
    void Insert(string x, string y){
        int r=hashFunction(x);
        Infos[r].Insert(x,y,r);
    }
    void Delete(string x){
        int r=hashFunction(x);
        Infos[r].Delete(x);
    }
};

int main(){
    read;
    write;
    int n;
    cin>>n;
    SymbolTable* S = new SymbolTable(n);
    string x,y,z;
    while(cin>>z){
        if(z=="I"){
            cin>>x>>y;
            S->Insert(x,y);
        }
        else if(z=="D"){
            cin>>x;
            S->Delete(x);
        }
        else if(z=="L"){
            cin>>x;
            S->LookUp(x);
        }
        else if(z=="P"){
            S->Print();
        }
        cout<<endl<<endl;
    }
    return 0;
}
