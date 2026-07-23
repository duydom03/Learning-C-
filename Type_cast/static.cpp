#include<iostream>

class Base{};
class Derived : public Base{} ;
class UnRelated{};

int main(){
    // 1. Ép kiểu dữ liệu cơ bản
    double pi = 3.14159;
    int int_pi = static_cast<int>(pi); // Chuyển an toàn: 3

    // 2. Upcasting trong kế thừa (Lớp con -> Lớp cha)
    Derived d;
    Base* b_ptr = static_cast<Base*>(&d); // Rất an toàn

    // 3. Downcasting nguy hiểm (Lớp cha -> Lớp con)
    Base real_base;
    // Bắt buộc trình biên dịch tin tưởng bạn, nhưng nếu dùng sai d_ptr sẽ gây crash!
    Derived* d_ptr = static_cast<Derived*>(&real_base); 

    // 4. Lỗi Compile-time: Ép kiểu giữa 2 lớp không liên quan
    // Unrelated* u_ptr = static_cast<Unrelated*>(&d); // COMPILE ERROR!

    return 0;
}
