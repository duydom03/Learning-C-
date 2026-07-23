
#include <iostream>
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::lock_guard, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <atomic>             // std::atomic
#include <chrono>             // std::chrono::seconds (dùng cho hàm sleep)

// DỮ LIỆU DÙNG CHUNG TRONG QUÁN ÁN

// 1. MUTEX: Khóa bàn pass món ăn
std::mutex kho_hang_mtx;

// 2. CONDITION VARIABLE: Chuông báo món
std::condition_variable chuong_bao;

// Biến trạng thái món ăn
bool co_mon_an = false;

// 3. ATOMIC: Bảng đếm số đĩa ăn (An toàn tuyệt đối giữa các luồng)
std::atomic<int> dem_so_dia(0);


// --- LUỒNG 1: NHÂN VIÊN BẾP ---
void nhan_vien_bep() {
    std::cout << "[Bếp]: Đang nấu món ăn...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Giả lập nấu trong 1 giây

    // Tăng biến atomic (không cần mutex)
    dem_so_dia.fetch_add(1); // Hoặc viết gọn là: dem_so_dia++;

    // Dùng std::lock_guard (Lock RAII) để đặt món ăn lên bàn
    {
        std::lock_guard<std::mutex> lock(kho_hang_mtx); // Tự động lock khi khai báo
        co_mon_an = true;
        std::cout << "[Bếp]: Nấu xong! Đã đặt món lên bàn pass.\n";
    } // Tự động unlock khi thoát khỏi ngoặc nhọn này

    // Bấm chuông báo cho Phục vụ
    std::cout << "[Bếp]: *Reng reng* (Bấm chuông báo)\n";
    chuong_bao.notify_one(); // Tương đương ring_bell()
}


// --- LUỒNG 2: NHÂN VIÊN PHỤC VỤ ---
void nhan_vien_phuc_vu() {
    // Condition Variable cần std::unique_lock vì nó cần lock/unlock linh hoạt khi ngủ
    std::unique_lock<std::mutex> lock(kho_hang_mtx);

    // std::condition_variable::wait nhận vào lock và 1 lambda kiểm tra điều kiện
    // Nếu co_mon_an == false, nó sẽ tự nhả lock và đi ngủ. Khi được notify, nó thức dậy và tự lấy lại lock.
    chuong_bao.wait(lock, [] { return co_mon_an; });

    std::cout << "[Phục vụ]: Thức dậy lấy món mang cho khách!\n";
}


// ===================================================
// CHƯƠNG TRÌNH CHÍNH
// ===================================================
int main() {
    std::cout << "--- QUÁN ÁN BẮT ĐẦU MỞ CỬA ---\n\n";

    // 4. STD::THREAD: Tạo 2 luồng thực thi song song
    std::thread t_phuc_vu(nhan_vien_phuc_vu);
    std::thread t_bep(nhan_vien_bep);

    // Chờ 2 luồng hoàn thành công việc
    t_phuc_vu.join();
    t_bep.join();

    std::cout << "\nTổng số đĩa đã chuẩn bị (Atomic): " << dem_so_dia.load() << "\n";
    std::cout << "--- QUÁN ÁN ĐÓNG CỬA ---\n";

    return 0;
}
