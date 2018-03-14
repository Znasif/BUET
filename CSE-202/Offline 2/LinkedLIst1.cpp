#include<iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

typedef bool boolean;

class Element
{
private:
    Element * next;
    Element * prev;
    int value;
public:
    void setNext(Element *n)
    {
        next = n;
    }
    Element * getNext()
    {
        return next;
    }
    void setPrev(Element *p)
    {
        prev  = p;
    }
    Element * getPrev()
    {
        return prev;
    }
    void setValue(int v)
    {
        value = v;
    }
    int getValue()
    {
        return value;
    }
};

class LinkedListBase
{

protected:
    Element * head;
    Element * tail;
    int size;

public:
    LinkedListBase()
    {
        head=new Element;
        tail=new Element;
        head->setNext(tail);
        tail->setPrev(head);
        LinkedListBase::size = 0;
    }
    int getSize()
    {
        return size;int x;
    }
    ~LinkedListBase()
    {
        delete head;
        delete tail;
    }
};

class LinkedList: public LinkedListBase
{
public:
    void add(Element e)
    {
        Element *n;
        n=new Element;
        n->setValue(e.getValue());
        if(LinkedListBase::size==0)
        {
            n->setNext(0);
            n->setPrev(0);
            tail=n;
            head=n;
            LinkedListBase::size+=1;
        }
        else
        {
            tail->setNext(n);
            n->setNext(0);
            n->setPrev(tail);
            tail=n;
            LinkedListBase::size+=1;
        }
    }
    void add(int index,Element element)
    {
        Element *e;
        e=new Element;
        e->setValue(element.getValue());
        if(index==0 && LinkedListBase::size==0)
        {
            add(element);
            return;
        }
        else if(index<0 || index>=LinkedListBase::size) return;
        else if(index==0)
        {
            e->setNext(head);
            head->setPrev(e);
            e->setPrev(0);
            head=e;
            LinkedListBase::size+=1;
            return;
        }
        int i=0;
        Element *q;
        q=head;
        while(i<LinkedListBase::size)
        {
            if(i==index) break;
            q=q->getNext();
            i+=1;
        }
        q->getPrev()->setNext(e);
        e->setPrev(q->getPrev());
        e->setNext(q);
        q->setPrev(e);
        LinkedListBase::size+=1;
        return;
    }
    void addFirst(Element e)
    {
        add(0,e);
        return;
    }
    void addLast(Element e)
    {
        add(e);
        return;
    }
    void clear()
    {
        Element *tmp,*T;
        tmp=head;
        while (tmp!=0)
        {
            T=tmp;
            tmp=tmp->getNext();
            delete T;
        }
        head=new Element;
        tail=new Element;
        head->setNext(tail);
        tail->setPrev(head);
        LinkedListBase::size = 0;
    }
    boolean contains(Element e)
    {
        Element *temp;
        temp = head;
        while (temp != 0)
        {
            if (temp->getValue() == e.getValue()) return true ;
            temp = temp->getNext();
        }
        return false;
    }
    Element get(int index)
    {
        int i=0;
        Element *q;
        q=head;
        if(index==0)return *head;
        while(i<LinkedListBase::size)
        {
            if(i==index) break;
            q=q->getNext();
            i+=1;
        }
        return *q;
    }
    Element getFirst()
    {
        return get(0);
    }
    Element getLast()
    {
        return get(LinkedListBase::size-1);
    }

    int indexOf(Element e)
    {
        Element *temp;
        temp = head;
        int i=0;
        while (temp != 0)
        {
            if (temp->getValue() == e.getValue()) return i;
            temp = temp->getNext();
            i++;
        }
        return -1;
    }
    int lastIndexOf(Element e)
    {
        Element *temp;
        temp = tail;
        int i=LinkedListBase::size-1;
        int value= e.getValue();
        while (temp != 0)
        {
            if (temp->getValue() == value) return i;
            temp = temp->getPrev();
            i-=1;
        }
        return -1;
    }

    Element remove(int index)
    {
        if(index>=LinkedListBase::size){
                cout<<"invalid"<<endl;
                exit(0);
        }
        Element *tmp;
        tmp=head;
        int i=0;

        while (tmp!=0)
        {
            if(i==index) break;
            tmp=tmp->getNext();
            i++;
        }
        if(tmp)
        {
            if(tmp==head)
            {
                head=tmp->getNext();
            }
            else
            {
                tmp->getPrev()->setNext(tmp->getNext());
            }
            if(tmp==tail)
            {
                tail=tmp->getPrev();
            }
            else
            {
                tmp->getNext()->setPrev(tmp->getPrev());
            }
        }
        LinkedListBase::size--;
        Element tt;
        tt=*tmp;
        delete tmp;
        return tt;
    }
    boolean remove(Element e)
    {
        Element *tmp;
        tmp=head;
        int i=0;
        while (tmp!=0)
        {
            if(tmp->getValue()==e.getValue()) break;
            tmp=tmp->getNext();
            i++;
        }
        if(tmp)
        {
            remove(i);
            return true;
        }
        return false;
    }
    Element removeFirst()
    {
        return remove(0);
    }
    boolean removeFirstOccurrence(Element e)
    {
        return remove(e);
    }
    Element removeLast()
    {
        return remove(LinkedListBase::size-1);
    }
    boolean removeLastOccurrence(Element e)
    {
        Element *tmp;
        tmp=tail;
        int i=LinkedListBase::size-1;
        int val=e.getValue();
        while (tmp!=0)
        {
            if(tmp->getValue()==val) break;
            tmp=tmp->getPrev();
            i--;
        }
        if(tmp)
        {
            remove(i);
            return true;
        }
        return false;
    }
    int size()
    {
        return LinkedListBase::size;
    }
    boolean isEmpty()
    {
        if(LinkedListBase::size==0) return true;
        return false;
    }
    string toString()
    {
        if(!LinkedListBase::size) return "";
        char str[100];
        int j,k;
        string temp;
        temp="";
        Element *tmp;
        tmp=head;
        while(tmp)
        {
            int x=tmp->getValue();
            j=0;
            if(!x)
            {
                str[j++]='0';
            }
            else
            {
                while(x)
                {
                    str[j++]=(x%10)+'0';
                    x/=10;
                }
            }
            str[j]=0;
            for(k=0; k<j/2; k++)
            {
                str[k]=str[k]^str[j-k-1];
                str[j-k-1]=str[k]^str[j-k-1];
                str[k]=str[k]^str[j-k-1];
            }
            if(tmp!=head)
                temp+=',';
            temp+=str;
            tmp=tmp->getNext();
        }
        return temp;
    }
};

// a very simple main
int main()
{
    LinkedList ll;
    Element e,b,c;
    int x,y,z,i;
    e.setValue(12);
    b.setValue(34);
    c.setValue(56);
    printf("1. index 2. setValue 3.add(e) 4.add(i,e) 5.addFirst(e)\n6.addLast(e) 7.clear 8.contains(e) 9.get(i)\n");
    printf("10.getFirst 11.getLast 12.indexOf(e) 13.lastIndexOf(e)\n14.remove(i) 15.remove(e) 16.removeFirst\n17.removeFirstOccurrence(e) 18.removeLast 19.removeLastOccurrence\n20.size 21.isEmpty ");
    cin>>y;
    while(y){
    if(y==1){cin>>i;}
    else if(y==2){cin>>x;e.setValue(x);}
    else if(y==3){ll.add(e);}
    else if(y==4){ll.add(i,e);}
    else if(y==5){ll.addFirst(e);}
    else if(y==6){ll.addLast(e);}
    else if(y==7){ll.clear();}
    else if(y==8){ll.contains(e);}
    else if(y==9){ll.get(i);}
    else if(y==10){ll.getFirst();}
    else if(y==11){ll.getLast();}
    else if(y==12){ll.indexOf(e);}
    else if(y==13){ll.lastIndexOf(e);}
    else if(y==14){ll.remove(i);}
    else if(y==15){ll.remove(e);}
    else if(y==16){ll.removeFirst();}
    else if(y==17){ll.removeFirstOccurrence(e);}
    else if(y==18){ll.removeLast();}
    else if(y==19){ll.removeLastOccurrence(e);}
    else if(y==20){ll.size();}
    else if(y==21){ll.isEmpty();}
    cout<<ll.toString()<<endl;
    printf("1. index 2. setValue 3.add(e) 4.add(i,e) 5.addFirst(e)\n6.addLast(e) 7.clear 8.contains(e) 9.get(i)\n");
    printf("10.getFirst 11.getLast 12.indexOf(e) 13.lastIndexOf(e)\n14.remove(i) 15.remove(e) 16.removeFirst\n17.removeFirstOccurrence(e) 18.removeLast 19.removeLastOccurrence\n20.size 21.isEmpty ");
    cin>>y;
    }
    return 0;
}
