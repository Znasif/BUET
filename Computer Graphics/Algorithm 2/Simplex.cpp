/*
 * Algorithm : Simplex ( Linear Programming )
 * Author : Simon Lo
 * Note: Simplex algorithm on augmented matrix a of dimension (m+1)x(n+1)
 * returns 1 if feasible, 0 if not feasible, -1 if unbounded
 * returns solution in b[] in original var order, max(f) in ret
 * form: maximize sum_j(a_mj*x_j)-a_mn s.t. sum_j(a_ij*x_j)<=a_in
 * in standard form.
 * To convert into standard form:
 * 1. if exists equality constraint, then replace by both >= and <=
 * 2. if variable x doesn't have nonnegativity constraint, then replace by
 * difference of 2 variables like x1-x2, where x1>=0, x2>=0
 * 3. for a>=b constraints, convert to -a<=-b
 * note: watch out for -0.0 in the solution, algorithm may cycle
 * EPS = 1e-7 may give wrong answer, 1e-10 is better
 */

#include <bits/stdc++.h>

using namespace std;

#define MAX 107
#define INF 1000000007
#define EPS (1e-12)

void Pivot( long m,long n,double **A,long *B,long *N,long r,long c )
{
    long i,j;
    swap( N[c],B[r] );
    A[r][c] = 1/A[r][c];
    for( j=0;j<=n;j++ ) if( j!=c ) A[r][j] *= A[r][c];
    for( i=0;i<=m;i++ ){
        if( i!=r ){
            for( j=0;j<=n;j++ ) if( j!=c ) A[i][j] -= A[i][c]*A[r][j];
            A[i][c] = -A[i][c]*A[r][c];
        }
    }
}

long Feasible( long m,long n,double **A,long *B,long *N )
{
    long r,c,i;
    double p,v;
    while( 1 ){
        for( p=INF,i=0;i<m;i++ ) if( A[i][n]<p ) p = A[r=i][n];
        if( p > -EPS ) return 1;
        for( p=0,i=0;i<n;i++ ) if( A[r][i]<p ) p = A[r][c=i];
        if( p > -EPS ) return 0;
        p = A[r][n]/A[r][c];
        for( i=r+1;i<m;i++ ){
            if( A[i][c] > EPS ){
                v = A[i][n]/A[i][c];
                if( v<p ) r=i,p=v;
            }
        }
        Pivot( m,n,A,B,N,r,c );
    }
}

long Simplex( long m,long n,double **A,double *b,double &Ret )
{
    long B[MAX+7],N[MAX+7],r,c,i;
    double p,v;
    for( i=0;i<n;i++ ) N[i] = i;
    for( i=0;i<m;i++ ) B[i] = n+i;
    if( !Feasible( m,n,A,B,N ) ) return 0;
    while( 1 ){
        for( p=0,i=0;i<n;i++ ) if( A[m][i] > p ) p = A[m][c=i];
        if( p<EPS ){
            for( i=0;i<n;i++ ) if( N[i]<n ) b[N[i]] = 0;
            for( i=0;i<m;i++ ) if( B[i]<n ) b[B[i]] = A[i][n];
            Ret = -A[m][n];
            return 1;
        }
        for( p=INF,i=0;i<m;i++ ){
            if( A[i][c] > EPS ){
                v = A[i][n]/A[i][c];
                if( v<p ) p = v,r = i;
            }
        }
        if( p==INF ) return -1;
        Pivot( m,n,A,B,N,r,c );
    }
}
