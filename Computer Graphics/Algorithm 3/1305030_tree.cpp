#include <bits/stdc++.h>

#define SPLAY_TREE 0
#define AVL_TREE 1
#define DEBUG 1
#define dbg(s) if(DEBUG) fout<<endl<<"DEBUG : "<<s<<endl

using namespace std;

typedef tuple<vector<string>, int, int> itms;

ofstream fout;

struct node{
    node* parent = nullptr;
    node* left = nullptr;
    node* right = nullptr;
    int key, height;

    node(int key){
        this->key = key;
    }

    itms print(){
        string label = to_string(key);
        vector<string> left_lines, right_lines, lines;
        int left_pos = 0, right_pos = 0, left_width = 0, right_width = 0, width, pos;
        int middle = label.length();
        middle = max(middle, 2);
        if(left){
            itms l = left->print();
            left_lines = get<0>(l); left_pos = get<1>(l); left_width = get<2>(l);
        }
        if(right){
            itms r = right->print();
            right_lines = get<0>(r); right_pos = get<1>(r); right_width = get<2>(r);
        }
        middle = max(middle, right_pos + left_width - left_pos + 1);
        pos = left_pos + middle/2;
        width = left_pos + middle + right_width - right_pos;
        int len = left_lines.size() - right_lines.size();
        if(len > 0){
            string str(right_width, ' ');
            while(len>0){
                right_lines.push_back(str);
                len--;
            }
        }
        else if(len < 0){
            len = -len;
            string str(left_width, ' ');
            while(len>0){
                left_lines.push_back(str);
                len--;
            }
        }
        int r = label.length();
        if ((middle - label.length()) % 2 == 1 && parent && this == parent->left && r < middle){
            label += '.';
        }
        r = (middle-r)/2;
        label.insert(label.begin(), r , '.');
        label.insert(label.end(), r, '.');
        r = label.length()-1;
        if(label[0] == '.') label[0] = ' ';
        if(label[r] == '.') label[r] = ' ';
        label.insert(label.begin(), left_pos, ' ');
        label.insert(label.end(), right_width - right_pos, ' ');
        lines.push_back(label);
        string ll = "/";
        ll.insert(ll.begin(), left_pos, ' ');
        ll.insert(ll.end(), middle-2, ' ');
        ll += "\\";
        ll.insert(ll.end(), right_width - right_pos, ' ');
        lines.push_back(ll);
        string rr(width - left_width - right_width, ' ');
        for(int i = 0; i < left_lines.size(); i++){
            lines.push_back(left_lines[i] + rr + right_lines[i]);
        }
        return make_tuple(lines, pos, width);
    }
};

typedef tuple<node*, node*> nodes;

struct tree{
    node* root = nullptr;
    int status;

    tree(int key, int status){
        this->status = status;
        root = new node(key);
    }

    void print(){
        if(root == nullptr) return;
        itms pr = root->print();
        vector<string> rp = get<0>(pr);
        for(int i=0; i<rp.size(); i++){
            fout<<endl<<rp[i];
        }
    }

    int getHeight(node* v){
        if(v == nullptr) return -1;
        return v->height;
    }

    void updateHeight(node* v){
        v->height = max(getHeight(v->left), getHeight(v->right)) + 1;
    }

    node* leftRotate(node* v){
        node* temp = v->right;
        if(temp == nullptr) return v;
        temp->parent = v->parent;
        v->right = temp->left;
        if(temp->left) temp->left->parent = v;
        if(temp->parent){
            if(temp->parent->left == v) temp->parent->left = temp;
            else temp->parent->right = temp;
        }
        temp->left = v;
        v->parent = temp;
        if(status == AVL_TREE){
            updateHeight(v);
            updateHeight(temp);
        }
        return temp;
    }

    node* rightRotate(node* v){
        node* temp = v->left;
        if(temp == nullptr) return v;
        temp->parent = v->parent;
        v->left = temp->right;
        if(temp->right) temp->right->parent = v;
        if(temp->parent){
            if(temp->parent->left == v) temp->parent->left = temp;
            else temp->parent->right = temp;
        }
        temp->right = v;
        v->parent = temp;
        if(status == AVL_TREE){
            updateHeight(v);
            updateHeight(temp);
        }
        return temp;
    }

    node* splay(node* v, int key){
        if(v == nullptr || v->key == key) return v;
        if(v->key > key){
            if(v->left == nullptr) return v;
            if(v->left->key == key) v = rightRotate(v);
            else if(v->left->key > key){
                v->left->left = splay(v->left->left, key);
                if(v->left->left) v->left->left->parent = v->left;
                v = rightRotate(v);
                v = rightRotate(v);
            }
            else{
                v->left->right = splay(v->left->right, key);
                if(v->left->right) v->left->right->parent = v->left;
                v->left = leftRotate(v->left);
                v = rightRotate(v);
            }
        }
        else{
            if(v->right == nullptr) return v;
            if(v->right->key == key) v = leftRotate(v);
            else if(v->right->key > key){
                v->right->left = splay(v->right->left, key);
                if(v->right->left) v->right->left->parent = v->right;
                v->right = rightRotate(v->right);
                v = leftRotate(v);
            }
            else{
                v->right->right = splay(v->right->right, key);
                if(v->right->right) v->right->right->parent = v->right;
                v = leftRotate(v);
                v = leftRotate(v);
            }
        }
        return v;
    }

    void balanceHeight(node* v){
        while(v){
            updateHeight(v);
            if(getHeight(v->left) >= 2 + getHeight(v->right)){
                if(getHeight(v->left->left) >= getHeight(v->left->right)) v = rightRotate(v);
                else{
                    v->left = leftRotate(v->left);
                    v = rightRotate(v);
                }
            }
            else if(getHeight(v->right) >= 2 + getHeight(v->left)){
                if(getHeight(v->right->right) >= getHeight(v->right->left)) v = leftRotate(v);
                else{
                    v->right = rightRotate(v->right);
                    v = leftRotate(v);
                }
            }
            if(v->parent == nullptr) root = v;
            v = v->parent;
        }
    }

    nodes split(node* sp, node* in){
        nodes ret;
        if(in->key <= sp->key){
            node* temp = sp->left;
            sp->left = nullptr;
            ret = make_tuple(temp, sp);
        }
        else{
            node* temp = sp->right;
            sp->right = nullptr;
            ret = make_tuple(sp, temp);
        }
        return ret;
    }

    node* join(node* l, node* r){
        if(l == nullptr) return r;
        l = splay(l, INFINITY);
        l->right = r;
        return l;
    }

    node* avl_search(node* v, int key){
        if(v == nullptr || v->key == key) return v;
        if(v->key > key) return avl_search(v->left, key);
        return avl_search(v->right, key);
    }

    node* avl_insert(node* v, node* in){
        if(v->key > in->key){
            if(v->left == nullptr){
                v->left = in;
                in->parent = v;
                return in;
            }
            return avl_insert(v->left, in);
        }
        else{
            if(v->right == nullptr){
                v->right = in;
                in->parent = v;
                return in;
            }
            return avl_insert(v->right, in);
        }
    }

    void avl_delete(node* temp){
        if(temp == nullptr) return;
        if(temp->left == nullptr){
            if(temp->parent){
                if(temp == temp->parent->left) temp->parent->left = temp->right;
                else temp->parent->right = temp->right;
            }
            else if(temp->parent == nullptr) root = temp->right;
            if(temp->right) temp->right->parent = temp->parent;
        }
        else if(temp->right == nullptr){
            if(temp->parent){
                if(temp == temp->parent->left) temp->parent->left = temp->left;
                else temp->parent->right = temp->left;
            }
            else if(temp->parent == nullptr) root = temp->left;
            if(temp->left) temp->left->parent = temp->parent;
        }
        else{
            node* v = temp->right;
            while(v->left){
                v = v->left;
            }
            int l;
            l = v->key;
            v->key = temp->key;
            temp->key = l;
            avl_delete(v);
        }
    }

    bool look(int key){
        if(status == SPLAY_TREE){
            root = splay(root, key);
            if(root->key != key) return false;
            return true;
        }
        else{
            node* temp = avl_search(root, key);
            if(temp && temp->key == key) return true;
            return false;
        }
    }

    void add(int key){
        node* temp = new node(key);
        if(root == nullptr){
            root = temp;
        }
        else if(status == SPLAY_TREE){
            root = splay(root, key);
            nodes ret = split(root, temp);
            temp->left = get<0>(ret);
            temp->right = get<1>(ret);
            if(temp->left) temp->left->parent = temp;
            if(temp->right) temp->right->parent = temp;
            root = temp;
        }
        else{
            temp = avl_insert(root, temp);
            balanceHeight(temp);
        }
    }

    void cut(int key){
        if(status == SPLAY_TREE){
            root = splay(root, key);
            if(root->key == key){
                root = join(root->left, root->right);
                if(root) root->parent = nullptr;
            }
        }
        else{
            node* temp = avl_search(root, key);
            if(temp && temp->key == key){
                avl_delete(temp);
                balanceHeight(temp->parent);
            }
        }
    }
};


int main(){
    int n, i, c;
    cout<<"Enter \'0\' for SPLAY_TREE or \'1\' for AVL_TREE : ";
    cin>>c;
    string fl = "1305030_";
    if(c == SPLAY_TREE) fl += "SPLAY_TREE.txt";
    else fl += "AVL_TREE.txt";
    fout.open(fl, ofstream::out);
    freopen("1305030_input.txt","r",stdin);
    cin>>n>>i;
    tree* tr = new tree(i, c);
    fout<<"new : "<<i<<endl;
    tr->print();
    while(n>0){
        cin>>c>>i;
        if(c==0){
            fout<<endl<<"insert : ";
            tr->add(i);
        }
        else if(c==1){
            fout<<endl<<"search : ";
            tr->look(i);
        }
        else if(c==2){
            fout<<endl<<"delete : ";
            tr->cut(i);
        }
        fout<<i<<endl;
        n--;
        tr->print();
    }
    fclose(stdin);
}
