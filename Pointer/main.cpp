#include <iostream>
using namespace std;

int main()
{   
    // int *ptr = new int;
	// *ptr = 10;

	// delete ptr;
	// ptr = nullptr;

	// // kiểm tra con trỏ trước khi cấp phát
	// // nếu ptr null (chưa được cấp phát), cấp phát ptr
	// if (!ptr)
	// 	ptr = new int;
    // *ptr = 20;

	// // không cần kiểm tra con trỏ khi xóa
	// // Nếu ptr không null, biến được cấp phát động sẽ bị xóa.
	// // Nếu ptr là null, không có gì sẽ xảy ra.
	// delete ptr;

    // 1. Cấp phát động cho 1 biến int duy nhất
    int* ptr = new int;
    *ptr = 10;

    cout << "Dia chi cua chinh bien pointer (&ptr): " << &ptr << endl;
    cout << "Gia tri tai vung nho ( *ptr ): " << *ptr << endl;
    cout << "Dia chi vung nho dang tro toi ( ptr ): " << ptr << endl;

    // GIẢI PHÓNG VÙNG NHỚ ĐƠN TRƯỚC KHÍ TÁI SỬ DỤNG PTR
    delete ptr; 

    // 2. Gán ptr sang một mảng động 5 phần tử (KHÔNG dùng lại int* ptr)
    ptr = new int[5]; 
    cout << "\nDia chi mang moi cap phat: " << ptr << endl;

    ptr[0] = 10;
    ptr[1] = 20;
    ptr[2] = 30;
    ptr[3] = 40;
    ptr[4] = 50;

    for (int i = 0; i < 5; i++)
    {
        cout << "Value " << i << ": " << ptr[i] << endl;
    }

    // GIẢI PHÓNG MẢNG ĐỘNG TRƯỚC KHÍ THOÁT
    delete[] ptr; 
    ptr = nullptr; // Gán về nullptr để tránh dùng nhầm con trỏ ma (dangling pointer)

    return 0;
}