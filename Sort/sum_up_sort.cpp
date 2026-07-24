#include<iostream>
#include<algorithm>
using namespace std ;

int sum(int n){
    int ans = 0;
    while (n)
    {
        ans+=n%10;
        n/=10;
    }
    return ans;
    
}

// bool cmp (int a , int b){
//     if (sum(a)<sum(b))
//     {
//         return true;
//     }else return false;
    
// }

// >2 tieu chi : cung tong thi nao lon hon dung truoc
// bool cmp (int a , int b){
//     if(sum(a) != sum(b))
//     {
//         return sum(a)<sum(b);
//     }else return a > b;
    
    
// }



// bool cmp(int a, int b){
// 	int r1 = a % 2, r2 = b % 2;
// 	if(r1 == 0 && r2 == 1) return true;
// 	if(r1 == 1 && r2 == 0) return false;
// 	if(r1 == 0 && r2 == 0) return a > b; // chan giam dan 
// 	if(r1 == 1 && r2 == 1) return a < b; // le tang dan
// }


// xep theo toa do
bool cmp (pair<int ,int> x , pair<int , int > y){
    if (x.first != y.first)
    {
        return x.first < y.first ;
    }else x.second > y.second;
    
}

int main(){
    // int a[] = {8321, 1002 , 6330, 300000 , 303 , 1233 , 90 , 3};
    // int n = 8 ;
    // sort(a,a+n ,cmp );
    // cout<<"Mang xep tong cua 1 so tang dan "<<endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout<<a[i]<< " ";
    // }

    pair<int, int> a[] = {{3, 1}, {3, 2}, {1, 4}, {4, 3}, {3, 3}, {1, 1}, {4, 2}};
	int n = 7;
	sort(a, a + n, cmp);
	cout << "Mang sau khi sap xep : \n";
	for(pair<int, int> it : a){
		cout << it.first << " " << it.second << endl;
	}


    return 0;
}