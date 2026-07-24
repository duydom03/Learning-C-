#include<iostream>
#include<algorithm>
using namespace std ;


bool cmp(int x ,int y){
    if (x<y){
        return true;
    }else return false;
}

int main(){
    int a[] = {1,8,3,6,0,2,4,6,7,4,2,22,1} ;
    int n = 13 ;
    sort(a,a+n ,cmp );
    cout<<"Mang xep lon---be"<<endl;

    for (int i = 0; i < n; i++)
    {
        cout<<a[i]<< " ";
    }
    return 0;   
    
}