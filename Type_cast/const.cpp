
#include <iostream>

void legacyCFunction(char* str) {
    std::cout << "Legacy C function: " << str << "\n";
}

int main() {
    // loai bo const -- bien a se dc thay doi thong qua b
    
    const int a = 11;
    int* b = const_cast<int*>(&a) ; // khi nay a se khon con la const nua va co the thay doi gtri dc
    

    // Trường hợp 1: Dùng hợp lệ (Thực chất biến gốc KHÔNG CONST, chỉ bị gán vào const reference)
    int nonConstValue = 100;
    const int& ref = nonConstValue; // x22xxx = 100

    // Sửa ref thông qua const_cast
    int& mutableRef = const_cast<int&>(ref); // mut = ref = x22xxx = 100
    mutableRef = 200; // Hợp lệ! nonConstValue giờ là 200.
    std::cout<<"nonConstValue : "<<nonConstValue<<"\n";

    // Trường hợp 2: Giao tiếp thư viện C cũ
    const char* text = "Hello World";
    legacyCFunction(const_cast<char*>(text)); // Bỏ const để truyền vào hàm C

    // // Trường hợp 3: NGUY HIỂM (Undefined Behavior)
    // const int originalConst = 50; // Khai báo hằng gốc
    // int* ptr = const_cast<int*>(&originalConst);
    // *ptr = 999; // LỖI UNDEFINED BEHAVIOR! (Giá trị có thể không thay đổi do Compiler Optimization)
}