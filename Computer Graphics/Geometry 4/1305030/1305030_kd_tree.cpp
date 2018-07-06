#include<vector>
#include<map>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<windows.h>
#include<algorithm>
#include<time.h>
#include<glut.h>
#include<tuple>
#define DEBUG 1
#define dbg(s) if(DEBUG) cout<<s

using namespace std;

ifstream fin("1305030_points.txt");
ofstream fout;

typedef tuple<vector<string>, int, int> itms;

int n, q;

struct Point{
    double x, y;
    int idx=-1;

    void print(){
        dbg(idx<<" X : "<< x << " Y : "<< y << '\n');
    }

    string gstr(){
        return to_string(idx)+" X : "+to_string(x).substr(0,5)+" Y : "+to_string(y).substr(0,5);
    }
};

struct query{
    int type = 0;
    Point a, b;
    void print(){
        if(type==0){
            dbg("Range : \n");
            a.print();
            b.print();
        }
        else{
            dbg("Nearest : \n");
            a.print();
        }
    }
};

vector<Point> A, B;
vector<query> Q;

bool compareByY(const Point &a, const Point &b)
{
    return (a.y < b.y) || ((a.y==b.y) && (a.x<b.x));
}

bool compareByX(const Point &a, const Point &b)
{
    return (a.x < b.x) || ((a.x==b.x) && (a.y<b.y));
}

struct node{
    node* parent = nullptr;
    node* left = nullptr;
    node* right = nullptr;
    int key;
    bool leaf = false;

    itms print(){
        string label = to_string(key);//A[key].gstr();
        if(leaf) label = "*"+ label;
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

bool inRange(query q, Point r){
    if(q.a.x<=r.x && q.b.x>=r.x && q.a.y<=r.y && q.b.y>=r.y){
        return true;
    }
    return false;
}

double distance(Point a, Point b){
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double distance(query q, Point r){
    if(q.a.x<=r.x && q.b.x>=r.x) return min(abs(q.a.y-r.y), abs(q.b.y-r.y));
    if(q.a.y<=r.y && q.b.y>=r.y) return min(abs(q.a.x-r.x), abs(q.b.x-r.x));
    if(q.a.x>r.x && q.a.y>r.y) return distance(q.a, r);
    if(q.b.x<r.x && q.b.y<r.y) return distance(q.b, r);
    if(q.a.x>r.x && q.b.y<r.y){
        Point a;
        a.x = q.a.x;
        a.y = q.b.y;
        return distance(a, r);
    }
    if(q.b.x<r.x && q.a.y>r.y){
        Point a;
        a.x = q.b.x;
        a.y = q.a.y;
        return distance(a, r);
    }
    cout<<"Some Mistake"<<endl;
    return -1;
}

bool compareRange(query q, query r){
    if(q.a.x>r.b.x || r.a.x>q.b.x) return false;
    if(q.a.y>r.b.y || r.a.y>q.b.y) return false;
    return true;
}

bool completelyIn(query q, query r){
    return inRange(q, r.a) && inRange(q, r.b);
}

struct tree{
    node* root = nullptr;
    double near_len = INFINITY;
    int members = 0;
    int near_point = -1;

    tree(){
        root = build(0, B.size()-1, 0);
    }

    void reportSub(node* v){
        if(v == nullptr) return;
        //cout<<v->key<<endl;
        if(v->leaf){
            cout<<"("<<A[v->key].x<<" , "<<A[v->key].y<<")";
            members++;
        }
        reportSub(v->left);
        reportSub(v->right);
    }

    void searchRange(node* v, query r, query q, int d){
        if(v == nullptr) return;
        if(v && v->leaf && inRange(q, A[v->key])){
            cout<<"("<<A[v->key].x<<" , "<<A[v->key].y<<")";
            members++;
            return;
        }
        query p1 = r, p2 = r;
        if(d%2){
            p1.b.y = A[v->key].y;
            p2.a.y = A[v->key].y;
        }
        else{
            p1.b.x = A[v->key].x;
            p2.a.x = A[v->key].x;
        }
        if(completelyIn(q, p1)) reportSub(v->left);
        else if(compareRange(q, p1)){
            //if(v->left) cout<<v->left->key<<endl;
            searchRange(v->left, p1, q, d+1);
        }
        if(completelyIn(q, p2)) reportSub(v->right);
        else if(compareRange(q, p2)){
            //if(v->right) cout<<v->right->key<<endl;
            searchRange(v->right, p2, q, d+1);
        }
    }

    void nearest(node* v, query r, Point q, int d){
        if(v == nullptr) return;
        if(v && v->leaf && near_len>distance(q, A[v->key])){
            near_len = distance(q, A[v->key]);
            near_point = v->key;
            return;
        }
        query p1 = r, p2 = r;
        if(d%2){
            p1.b.y = A[v->key].y;
            p2.a.y = A[v->key].y;
        }
        else{
            p1.b.x = A[v->key].x;
            p2.a.x = A[v->key].x;
        }
        if(inRange(p1, q)){
            nearest(v->left, p1, q, d+1);
            if(v->right && near_len>distance(p2, q)) nearest(v->right, p2, q, d+1);
        }
        else{
            nearest(v->right, p2, q, d+1);
            if(v->left && near_len>=distance(p1, q)) nearest(v->left, p1, q, d+1);
        }
    }

    void print(){
        if(root == nullptr) return;
        itms pr = root->print();
        vector<string> rp = get<0>(pr);
        for(int i=0; i<rp.size(); i++){
            dbg(endl<<rp[i]);
        }
    }

    node* build(int i, int j, int d){
        node *ret = new node();
        int mid;
        if(i==j){
            ret->key = B[j].idx;
            //dbg(i<<endl);
            ret->leaf = true;
            return ret;
        }
        else if(d%2){
            sort(B.begin()+i, B.begin()+j+1, compareByY);
        }
        else{
            sort(B.begin()+i, B.begin()+j+1, compareByX);
        }
        mid = (i+j)/2;
        //dbg("("<<i<<", "<<mid-1<<")"<<"("<<mid+1<<", "<<j<<")"<<endl);
        ret->key = B[mid].idx;
        //dbg(mid<<endl);
        ret->left = build(i, mid, d+1);
        ret->left->parent = ret;
        ret->right = build(mid+1, j, d+1);
        ret->right->parent = ret;
        return ret;
    }
};

tree* tr;
query plane;

void input(){
    fin>>n;
    fout.open("1305030_out.txt", ofstream::out);
    for(int i=0; i<n; i++){
        Point a;
        fin>>a.x>>a.y;
        a.idx = i;
        A.push_back(a);
        B.push_back(a);
    }

    tr = new tree();
    tr->print();

    plane.a.x = -INFINITY;
    plane.a.y = -INFINITY;
    plane.b.x = INFINITY;
    plane.b.y = INFINITY;

    fin>>q;

    for(int i=0; i<q; i++){
        char p;
        fin>>p;
        query r;
        if(p=='R'){
            r.type = 0;
            fin>>r.a.x>>r.a.y;
            fin>>r.b.x>>r.b.y;
            Q.push_back(r);
        }
        else{
            r.type = 1;
            fin>>r.a.x>>r.a.y;
            Q.push_back(r);
        }
    }
    dbg("\n*********POINTS********\n");
    for(int i=0; i<n; i++) A[i].print();
    dbg("*********QUERIES********\n");
}
