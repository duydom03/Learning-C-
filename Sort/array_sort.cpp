#include <iostream>
#include <algorithm> 

using namespace std;

// Hàm so sánh để xếp LỚN -> BÉ (Giảm dần)
bool cmp(int x, int y) {
    return x > y; // Sửa < thành >
}

int main() {
    int a[] = {1, 8, 3, 6, 0, 2, 4, 6, 7, 4, 2, 22, 1};
    
    // Tính số lượng phần tử trong mảng tĩnh a
    int n = sizeof(a) / sizeof(a[0]);

    // Sắp xếp giảm dần từ index 0 đến n-1
    sort(a, a + n, cmp);

    // sort(a , a+n , greater<int>());

    // sort(a, a + n, [](int x, int y) {
    //     return x > y;
    // });

    cout << "Mang xep lon---be:" << endl;

    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    return 0;   
}