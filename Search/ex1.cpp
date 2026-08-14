#include <iostream>
#include <vector>

// 1. Chèn k vào mảng đã sắp xếp (dùng & để sửa trực tiếp vector gốc)
void SearchInsertK(std::vector<int>& arr, int k) { 
    for (int i = 0; i < arr.size(); i++) { 
        if (arr[i] >= k) { 
            // Dùng hàm insert của vector để chèn k vào vị trí i
            arr.insert(arr.begin() + i, k); 
            return;
        } 
    } 
    // Nếu k lớn hơn tất cả phần tử, chèn vào cuối
    arr.push_back(k); 
} 

// 2. Tìm kiếm và xóa phần tử x khỏi vector
bool findAndDelete(std::vector<int>& arr, int x) {
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        if (*it == x) {
            arr.erase(it); // Xóa phần tử tại vị trí con trỏ (iterator) it
            return true;   // Xóa thành công
        }
    }
    return false; // Không tìm thấy x
}

// Hàm bổ trợ in vector
void printVector(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() { 
    std::vector<int> arr = {1, 3, 5, 6}; 
    int k = 2; 

    std::cout << "Vector ban dau: ";
    printVector(arr);

    // --- THỰC HIỆN CHÈN ---
    SearchInsertK(arr, k);
    std::cout << "Sau khi chen " << k << ": ";
    printVector(arr);

    // --- THỰC HIỆN TÌM KÍẾM VÀ XÓA ---
    int x = 5;
    std::cout << "\nDang tim va xoa phan tu " << x << "..." << std::endl;
    if (findAndDelete(arr, x)) {
        std::cout << "Da xoa thanh cong! Vector hien tai: ";
        printVector(arr);
    } else {
        std::cout << "Khong tim thay " << x << " trong vector." << std::endl;
    }

    return 0; 
}