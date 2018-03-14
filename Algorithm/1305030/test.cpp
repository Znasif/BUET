#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstdio>

using namespace std;

struct point {
    int x;
    int y;
};

double distanc( point a, point b )
{
    double dist;

    dist = sqrt( ( a.x - b.x )*( a.x - b.x ) + ( a.y - b.y )*( a.y - b.y ) );

    return dist;
}

bool sort_x( struct point a, struct point b )
{
    return a.x < b.x;
}

bool sort_y( struct point a, struct point b )
{
    return a.y < b.y;
}

void sort_xy( vector<point> &X, vector<point> &Y )
{
    sort( X.begin(), X.end(), sort_x );
    sort( Y.begin(), Y.end(), sort_y );

    return;
}

double closest_pair_simple( vector<point> P )
{
    double dist;
    double d;
    int i;
    int j;

    dist = INT_MAX;

    for( i = 0; i < P.size(); i++ ) {
        for( j = 0; j < P.size(); j++ ) {
            if( j != i ) {
                d = distanc( P[i], P[j] );
                if( d < dist ) {
                    dist = d;
                }
            }
        }
    }

    return dist;

}

vector< vector<point> > partition( vector<point> X, int l )
{
    vector< vector<point> > V;
    vector<point> T1;
    vector<point> T2;
    int i;

    for( i = 0; i < X.size(); i++ ) {
        if( X[i].x < l ) {
            T1.push_back(X[i]);
        } else {
            T2.push_back(X[i]);
        }
    }

    V.push_back(T1);
    V.push_back(T2);

    return V;
}

void disp( vector<point> P )
{
    int i;
    for( i = 0; i < P.size(); i++ ) {
        cout << "( " << P[i].x << ", " << P[i].y << ")\n";
    }
    cout << endl;
}

double closest_pair( vector<point> P, vector<point> X, vector<point> Y )
{
    double dist;
    int l;
    int i;
    int j;
    int k;
    double d1;
    double d2;
    double t;
    vector< vector<point> > V;
    vector<point> PL, PR, XL, XR, YL, YR, Y1;

    if( P.size() <= 3 ) {
        dist = closest_pair_simple( P );
        return dist;
    } else {
        l = X[X.size()/2].x;
        V = partition( P, l );
        PL = V[0];
        PR = V[1];

        V = partition( X, l );
        XL = V[0];
        XR = V[1];

        V = partition( Y, l );
        YL = V[0];
        YR = V[1];

        d1 = closest_pair( PL, XL, YL );
        d2 = closest_pair( PR, XR, YR );

        if( d1 > d2 ) {
            dist = d1;
        } else {
            dist = d2;
        }

        for( i = 0; i < Y.size(); i++ ) {
            if( Y[i].x >= l-dist && Y[i].x <= l+dist ) {
                Y1.push_back( Y[i] );
            }
        }

        for( i = 0; i < Y1.size(); i++ ) {
            for( j = 0; j < Y1.size(); j++ ) {
                if( j != i ) {
                    d1 = distanc( Y1[i], Y1[j] );
                    if( d1 < dist ) {
                        dist = d1;
                    }
                }
            }
        }

        return dist;

    }
}

int main()
{
    freopen("input.txt","r",stdin);
    int n;
    int i;
    int a;
    int b;
    point p;
    double dist;
    double d;
    vector< point > P;
    vector< point > X;
    vector< point > Y;

    cout << "Enter the numbers of points: ";
    cin >> n;
    cout << "Enter the points(x,y): ";

    for( i = 0; i < n; i++ ) {

        cin >> a >> b;

        p.x = a;
        p.y = b;

        P.push_back(p);
        X.push_back(p);
        Y.push_back(p);
    }

    p.x = 0;
    p.y = 0;

    sort_xy( X, Y );

    dist = closest_pair( P, X, Y );

    cout << "The sortest distance is: " << dist << endl;

    return 0;
}
