#include<bits/stdc++.h>

using namespace std;


#define ms(x,val) memset(x,val,sizeof(x))
#define scan(x) scanf("%d",&x)
#define scanL(x) scanf("%I64d",&x)
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
#define eps 1e-12
#define inf 10
#define MAXN 1000009
#define MOD 1000000007 // 10^9 + 7
#define first x
#define second y


template < class T > T prnt( T a  ) { cout<<(a)<<endl; return a; }

template < class T > T gcd(T a , T b ) { if(b==0) return a; else return gcd(b, a%b);}
template < class T > T lcm(T a , T b ) { return  a*b / gcd(a, b);}
template < class T > T absolute(T a ) { if(a>0) return a; else return -a;}
inline iii power(iii base,iii p) { iii ans=1; while(p>0) ans*=base,p-=1; return ans;}


struct point{
	int ID;
	double x,y;
	point(double x,double y){
		this->x = x;
		this->y = y;
	}

	point(){

	}

	bool operator==(const point &temp)const{
		if(abs(temp.x - this->x) < eps && abs(temp.y - this->y) < eps) return true;
		return false;	
	}
	void print(){
		cout<<"Point-> "<<ID<<": "<< x <<", "<<y<<endl;
	}
};

struct edge{
	int ID;
	int s,e,a,b;
	double st,ed,ta,tb;
	bool valid;
	// edge(double st, double ed, double ta, double tb){
	// 	this->st = st;
	// 	this->ed = ed;
	// 	this->ta = ta;
	// 	this->tb = tb;
	// 	valid = true;
	// }

	edge(int s, int e, int a, int b){
		this->s = s;
		this->e = e;
		this->a = a;
		this->b = b;
		valid = true;
	}

	edge(){
		s = e = a = b = -1;
		valid = true;
	}


	void print(){
		cout<<"Edge-> "<<ID<<": "<< s<<", "<<e<<", "<<a<<", "<<b<<endl;
	}
};

struct vec{
	int ID;
	double a,b;
	vec(double a, double b){
		this->a = a;
		this->b = b;
	}

	vec(){

	}
	vec(point c, point d){
		this->a = d.x - c.x;
		this->b = d.y - c.y;
	}

	void print(){
		cout<<"Vector-> "<<a<<", " <<b<<endl;
	}
};




int n;
point topPoint,left_extreme_point,right_extreme_point,leftExtremePoint,rightExtremePoint;
vector<point> pVector;
vector<edge> eVector;
map< pair<int,int>, int> eMap;


struct Triangle{
	point p[3];
	// int color[3];
	Triangle(int i, int j, int k){
		p[0] = pVector[i];
		p[1] = pVector[j];
		p[2] = pVector[k];
	}
	Triangle(){

	}
};


point find_top_most_point(){
	point ret;
	ret.y = -inf;
	for(int i = 0; i < pVector.size(); i++){
		point temp = pVector[i];
		if(temp.y > ret.y){
			ret = temp;
		}	
	}
	return ret;
}

// vec crossProduct(vec a, vec b){
// 	vec cross;
// 	cross.x = a.y * b.z - a.z * b.y;
// 	cross.y = b.x * a.z - a.x * b.z;
// 	cross.z = a.x * b.y - a.y * b.x;

// 	return cross;
// }


bool find_turn(point x, point y,point q)
{
	point a,b;
	a.first = y.first-x.first;
	a.second = y.second-x.second;

	b.first = q.first-x.first;
	b.second = q.second-x.second;

	double val = a.first*b.second-a.second*b.first;
	if(val>0) return true;
	return false;
}


point find_left_extreme_point(point T){

	point ref = pVector[0] == T? pVector[1]:pVector[0];

	for(int i = 0; i < pVector.size(); i++){
		if(pVector[i] == T) continue;
		if(find_turn(T,ref,pVector[i]) == false){
			ref = pVector[i];
		}
	}
	return ref;
}


point find_right_extreme_point(point T){

	point ref = (pVector[0] == T? pVector[1]:pVector[0]);

	for(int i = 0; i < pVector.size(); i++){
		if(pVector[i] == T) continue;
		if(find_turn(T,ref,pVector[i]) == true){
			ref = pVector[i];
		}
	}
	return ref;
}


double calc_area(double x1, double y1, double x2, double y2, double x3, double y3)
{
   return absolute((x1*(y2-y3) + x2*(y3-y1)+ x3*(y1-y2))/2.0);
}


bool in_triangle(Triangle t, int pid){
	double x,y;
	x = pVector[pid].x;
	y = pVector[pid].y;
	// cout<<x<<' '<<y<<endl;
	double area_triangle = calc_area(t.p[0].x,t.p[0].y,t.p[1].x,t.p[1].y,t.p[2].x,t.p[2].y);
	// cout<<area_triangle<<' ';
	double a,b,c;
	a = calc_area(t.p[0].x,t.p[0].y,t.p[1].x,t.p[1].y,x,y);
	b = calc_area(t.p[0].x,t.p[0].y,t.p[2].x,t.p[2].y,x,y);
	c = calc_area(t.p[2].x,t.p[2].y,t.p[1].x,t.p[1].y,x,y);

	// cout<<a<<' '<<b<<' '<<c<<endl;

	if(absolute( area_triangle - a - b - c )< eps) return true;
	// cout<<absolute(area_triangle-a-b-c)<<" not true"<<endl;
	return false;
}

bool onEdge(int pid, int E){
	point a = pVector[pid];
	edge eg = eVector[E];

	if(calc_area(a.x,a.y,pVector[eg.s].x,pVector[eg.s].y,pVector[eg.e].x,pVector[eg.e].y) < eps){
		return true;
	}
	return false;
}

bool chk(int i){
	point a,b;
	a = pVector[eVector[i].s];
	b = pVector[eVector[i].e];
	if(abs(a.x) > 30 || abs(a.y) > 30 || abs(b.x) > 30 || abs(b.y) > 30) return false;
	return true;
}



void printEdge(){
	freopen("OpenGL_CodeBlocks/out.txt","w",stdout);

	cout<<eVector.size()<<endl;
	for(int i = 0; i < eVector.size(); i++){
		if(eVector[i].valid == false) continue;
		if(chk(i) == false) continue;
		// printf("%d : %d %d\n", eVector[i].ID,eVector[i].s, eVector[i].e);
		printf("%.3lf %.3lf %.3lf %.3lf\n ", pVector[eVector[i].s].x, pVector[eVector[i].s].y,pVector[eVector[i].e].x, pVector[eVector[i].e].y);
	}
	cout<<endl;
	fclose(stdout);
}


void initialize(){
	edge temp;
	temp.ID = eVector.size();
	temp.s = topPoint.ID;
	temp.e = leftExtremePoint.ID;
	temp.a = rightExtremePoint.ID;
	temp.b = -1;
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	eVector.pb(temp);
	eVector[0].print();

	temp.ID = eVector.size();
	temp.e = rightExtremePoint.ID;
	temp.a = leftExtremePoint.ID;
	temp.b = -1;
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	eVector.pb(temp);

	temp.ID = eVector.size();
	temp.s = leftExtremePoint.ID;
	temp.a = topPoint.ID;
	temp.b = -1;
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	eVector.pb(temp);
}

double calc_angle(vec x, vec y){
	// cout<<x.a<<' '<<x.b<<endl;
	// cout<<y.a<<' '<<y.b<<endl;
	double crossValue = x.a * y.b - y.a * x.b;
	double dotValue = x.a * y.a + x.b * y.b;

	return abs(atan2(crossValue , dotValue) * 180.0 / pi);
}


void legalize(int pr, int E){

	cout<<"legalize: "<<pr<<' '<<E<<endl;

	edge eg = eVector[E];
	int tid;
	if(eg.a == pr){
		tid = eg.b;
	}
	else tid = eg.a;

	cout<<"tid: "<<tid<<endl;
	if(tid == -1) return;

	vec x1(pVector[pr],pVector[eg.s]),x2(pVector[pr],pVector[eg.e]),y1(pVector[tid],pVector[eg.s]),y2(pVector[tid],pVector[eg.e]);
	double angle = calc_angle(x1,x2) + calc_angle(y2,y1);
	cout<<pr<<' '<<eg.s<<'-'<<eg.e<<'-'<<tid<<' '<<eg.s<<'-'<<tid<<' '<<eg.e<<endl;
	cout<<"angle: "<<calc_angle(x1,x2)<<' '<<calc_angle(y1,y2)<<endl;
	if(angle < 180.0) return;

	if(E < 3) cout<<"aaaaaaaaaaaaaa"<<endl;
	eVector[E].valid = false;

	edge new_eg;
	new_eg.ID = eVector.size();
	new_eg.s = pr;
	new_eg.e = tid;
	new_eg.a = eg.s;
	new_eg.b = eg.e;
	// new_eg.valid = true;
	eMap[MP(new_eg.s,new_eg.e)] = eMap[MP(new_eg.e,new_eg.s)] = new_eg.ID;
	eVector.pb(new_eg);

	cout<<"legalized"<<endl;
	int idx = eMap[MP(pr,eg.s)];
	if(eVector[idx].a == eg.e){
		eVector[idx].a = tid;
	}
	else{
		eVector[idx].b = tid;
	}
	eVector[idx].print();

	idx = eMap[MP(pr,eg.e)];
	if(eVector[idx].a == eg.s){
		eVector[idx].a = tid;
	}
	else{
		eVector[idx].b = tid;
	}
	eVector[idx].print();

	idx = eMap[MP(eg.s,tid)];
	if(eVector[idx].a == eg.e){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}
	eVector[idx].print();

	idx = eMap[MP(eg.e,tid)];
	if(eVector[idx].a == eg.s){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}
	eVector[idx].print();

	legalize(pr,eMap[MP(eg.s,tid)]);
	legalize(pr,eMap[MP(eg.e,tid)]);





}




void updateNlegalize(int pr, int E, int tid){
	cout<<"updated "<<tid<<endl;

	edge eg = eVector[E];
	// eg.print();
	// cout<<pr<<endl;
	int idx = eMap[MP(eg.s,eg.e)];
	if(eVector[idx].a == tid){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}
	eVector[idx].print();
	// legalize(pr,idx);

	idx = eMap[MP(eg.s,tid)];
	if(eVector[idx].a == eg.e){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}
	eVector[idx].print();
	// legalize(pr,idx);

	idx = eMap[MP(eg.e,tid)];
	cout<<idx<<' '<<eg.e<<' '<<tid<<endl;
	if(eVector[idx].a == eg.s){
		eVector[idx].a = pr;

	}
	else{
		eVector[idx].b = pr;
	}
	eVector[idx].print();
	legalize(pr,eMap[MP(eg.s,eg.e)]);
	legalize(pr,eMap[MP(eg.s,tid)]);
	legalize(pr,idx);
}




void build_normal_edge(int pr, int E, int tid ){
	// cout<<"noob: \n"<<pr<<' ';
	// eVector[E].print();
	cout<<"Building edge: "<<endl;
	edge temp,eg;
	eg = eVector[E];

	temp.ID = eVector.size();

	temp.s = pr;
	temp.e = eg.s;
	temp.a = eg.e;
	temp.b = tid;

	eVector.pb(temp);
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	temp.print();

	temp.ID = eVector.size();

	temp.s = pr;
	temp.e = eg.e;
	temp.a = eg.s;
	temp.b = tid;

	eVector.pb(temp);
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	temp.print();

	temp.ID = eVector.size();

	temp.s = pr;
	temp.e = tid;
	temp.a = eg.s;
	temp.b = eg.e;

	eVector.pb(temp);
	eMap[MP(temp.s,temp.e)] = eMap[MP(temp.e,temp.s)] = temp.ID;
	temp.print();

	updateNlegalize(pr, E, tid);

}

void build_abnormal_edge(int pr, int E){
	eVector[E].valid = false;
	edge eg = eVector[E];
	edge new_eg;
	new_eg.s = pr;
	new_eg.e = eg.s;
	new_eg.a = eg.a;
	new_eg.b = eg.b;
	new_eg.ID = eVector.size();
	eMap[MP(pr,eg.s)] = eMap[MP(eg.s,pr)] = new_eg.ID;
	eVector.pb(new_eg);

	new_eg.s = pr;
	new_eg.e = eg.e;
	new_eg.a = eg.a;
	new_eg.b = eg.b;
	new_eg.ID = eVector.size();
	eMap[MP(pr,eg.e)] = eMap[MP(eg.e,pr)] = new_eg.ID;
	eVector.pb(new_eg);

	new_eg.s = pr;
	new_eg.e = eg.a;
	new_eg.a = eg.s;
	new_eg.b = eg.e;
	new_eg.ID = eVector.size();
	eMap[MP(pr,eg.a)] = eMap[MP(eg.a,pr)] = new_eg.ID;
	eVector.pb(new_eg);

	new_eg.s = pr;
	new_eg.e = eg.b;
	new_eg.a = eg.e;
	new_eg.b = eg.s;
	new_eg.ID = eVector.size();
	eMap[MP(pr,eg.b)] = eMap[MP(eg.b,pr)] = new_eg.ID;
	eVector.pb(new_eg);

	int idx = eMap[MP(eg.s,eg.a)];
	if(eVector[idx].a == eg.e){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}

	idx = eMap[MP(eg.e,eg.a)];
	if(eVector[idx].a == eg.s){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}

	idx = eMap[MP(eg.e,eg.b)];
	if(eVector[idx].a == eg.s){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}

	idx = eMap[MP(eg.s,eg.b)];
	if(eVector[idx].a == eg.s){
		eVector[idx].a = pr;
	}
	else{
		eVector[idx].b = pr;
	}


	legalize(pr,eMap[MP(eg.s,eg.a)]);
	legalize(pr,eMap[MP(eg.s,eg.b)]);
	legalize(pr,eMap[MP(eg.e,eg.a)]);
	legalize(pr,eMap[MP(eg.e,eg.b)]);
}


bool do_it(int pr, int E){

	cout<<"in do it: "<<pr<<' '<<E<<endl;


	edge eg = eVector[E];
	eg.print();
	if(eg.a != -1){
		Triangle T(eg.s, eg.e, eg.a);
		if(in_triangle(T, pr) == true){
			if(onEdge(pr,E) == false){
				cout<<"here"<<endl;
				build_normal_edge(pr, E, eg.a);
				return true;
			}
			else{
				build_abnormal_edge(pr,E);
				cout<<"ashce"<<endl;
				cout<<E<<' '<<pr<<endl;
				eVector[E].print();
				return true;
			}
		}
	}
	
	if(eg.b != -1){
		Triangle T(eg.s, eg.e, eg.b);
		if(in_triangle(T,pr) == true){
			if(onEdge(pr,E) == false){
				cout<<"here2"<<endl;
				build_normal_edge(pr, E, eg.b);
				return true;
			}
			else{
				cout<<"ashce2"<<endl;
				build_abnormal_edge(pr,E);
				return true;
			}
		}
	}
	return false;
}





void triangulate(){
	

	topPoint = find_top_most_point();
	left_extreme_point = find_left_extreme_point(topPoint);
	right_extreme_point = find_right_extreme_point(topPoint);

	topPoint.print();
	left_extreme_point.print();
	right_extreme_point.print();

	left_extreme_point.x--;
	left_extreme_point.y++;
	right_extreme_point.x++;
	right_extreme_point.y++;

	
	vec leftVector(topPoint,left_extreme_point), rightVector(topPoint,right_extreme_point);

	
	leftExtremePoint.x = leftVector.a * inf + topPoint.x;
	leftExtremePoint.y = leftVector.b * inf + topPoint.y;

	rightExtremePoint.x = rightVector.a * inf + topPoint.x;
	rightExtremePoint.y = rightVector.b * inf + topPoint.y;

	leftExtremePoint.ID = pVector.size();
	pVector.pb(leftExtremePoint);

	rightExtremePoint.ID = pVector.size();
	pVector.pb(rightExtremePoint);

	leftExtremePoint.print();
	rightExtremePoint.print();


	initialize();

	for(int i = 0; i < pVector.size() - 2; i++){
		if(pVector[i] == topPoint) continue;
		bool flag = false;	
		// cout<<"i: "<<i<<endl;
		for(int j = 0; j < eVector.size(); j++){
			if(eVector[j].valid){

				flag = do_it(pVector[i].ID, eVector[j].ID);
			}
			// else{
			// 	cout<<"not valid"<<' ';
			// }
			if(flag) break;
		}
		// cout<<endl;
		
	}

	printEdge();

	// Triangle T(0,1,2);
	// cout<<"result:"<<endl;
	// cout<<in_triangle(T,5)<<endl;

}



int main(){
	
	freopen("in.txt","r",stdin);
	double a,b;
	cin>>n;
	for(int i = 0; i < n; i++){
		cin>>a>>b;
		pVector.pb(point(a,b));
		pVector[i].ID = i;
	}
	fclose(stdin);

	triangulate();

	return 0;
}