#include <iostream>
#include <vector>
using namespace std;

// tim vi tri chen k vao mang arr

int searchInsertK(vector<int> arr, int k) {  
    for(int i = 0; i < arr.size(); i++) {  
       
        // if k is found or needs to be 
        // inserted before arr[i]
        if(arr[i] >= k) {  
            return i;  
        }  
    }  
    
    // if k is greater than all 
    // elements insert at the end
    return arr.size();  
}  

int main() {  
    vector<int> arr = {1, 3, 5, 6};  
    int k = 4;  
    cout << searchInsertK(arr, k) << endl;  
    for (int i= 0 ; i < arr.size(); i++)
    {
        cout << arr[i] << " ";
    }
    
    return 0;  
}