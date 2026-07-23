#include <iostream>
#include <pthread.h>
#include <unistd.h> // Dùng cho hàm sleep()

// 1. ATOMIC: Bảng đếm đơn giản (không sợ 2 người bấm cùng lúc)
class CustomAtomicInt {
private:
    volatile int value;
public:
    CustomAtomicInt(int val = 0) : value(val) {}
    void add_one() {
        __atomic_fetch_add(&value, 1, __ATOMIC_SEQ_CST); // Tăng 1 nguyên tử
    }
    int get() const {
        return __atomic_load_n(&value, __ATOMIC_SEQ_CST);
    }
};

// 2. MUTEX: Khóa kho hàng
class CustomMutex {
public:
    pthread_mutex_t raw_mtx;
    CustomMutex() { pthread_mutex_init(&raw_mtx, nullptr); }
    ~CustomMutex() { pthread_mutex_destroy(&raw_mtx); }
    void lock() { pthread_mutex_lock(&raw_mtx); }
    void unlock() { pthread_mutex_unlock(&raw_mtx); }
};

// 3. LOCK: Quy tắc vào kho tự động trả chìa khi ra
class CustomLock {
private:
    CustomMutex& mtx;
public:
    CustomLock(CustomMutex& m) : mtx(m) { mtx.lock(); }   // Bước vào -> Khóa
    ~CustomLock() { mtx.unlock(); }                      // Bước ra -> Tự mở
};

// 4. CONDITION VARIABLE: Chuông báo món
class CustomCV {
private:
    pthread_cond_t cond;
public:
    CustomCV() { pthread_cond_init(&cond, nullptr); }
    ~CustomCV() { pthread_cond_destroy(&cond); }
    void wait(CustomMutex& mtx) { pthread_cond_wait(&cond, &mtx.raw_mtx); }
    void ring_bell() { pthread_cond_signal(&cond); }
};

// 5. THREAD: Nhân viên làm việc
class CustomThread {
private:
    pthread_t id;
public:
    CustomThread(void* (*func)(void*)) {
        pthread_create(&id, nullptr, func, nullptr);
    }
    void wait_finish() {
        pthread_join(id, nullptr);
    }
};

// ===================================================
// DỮ LIỆU DÙNG CHUNG TRONG QUÁN ÁN
// ===================================================
CustomMutex    kho_hang_mtx;
CustomCV       chuong_bao;
bool           co_mon_an = false;
CustomAtomicInt dem_so_dia(0);

// --- LUỒNG 1: NHÂN VIÊN BẾP ---
void* nhan_vien_bep(void* arg) {
    std::cout << "[Bếp]: Đang nấu món ăn...\n";
    sleep(1); // Giả lập thời gian nấu 1 giây

    // Tăng đếm đĩa (Atomic)
    dem_so_dia.add_one();

    // Dùng Lock để vào kho đặt món ăn lên bàn
    {
        CustomLock lock(kho_hang_mtx);
        co_mon_an = true;
        std::cout << "[Bếp]: Nấu xong! Đã đặt món lên bàn pass.\n";
    }

    // Bấm chuông báo cho Phục vụ
    std::cout << "[Bếp]: *Reng reng* (Bấm chuông báo)\n";
    chuong_bao.ring_bell();

    return nullptr;
}

// --- LUỒNG 2: NHÂN VIÊN PHỤC VỤ ---
void* nhan_vien_phuc_vu(void* arg) {
    CustomLock lock(kho_hang_mtx);

    // Nếu bếp chưa nấu xong -> Ngồi chờ tiếng chuông
    while (!co_mon_an) {
        std::cout << "[Phục vụ]: Chưa có món, ngồi chờ tiếng chuông...\n";
        chuong_bao.wait(kho_hang_mtx); // Tự giải phóng lock để bếp làm việc và ngủ chờ
    }

    std::cout << "[Phục vụ]: Thức dậy lấy món mang cho khách!\n";
    return nullptr;
}

// ===================================================
// CHƯƠNG TRÌNH CHÍNH
// ===================================================
int main() {
    std::cout << "--- QUÁN ĂN BẮT ĐẦU MỞ CỬA ---\n\n";

    // Tạo 2 nhân viên (2 luồng) chạy song song
    CustomThread phuc_vu(nhan_vien_phuc_vu);
    CustomThread bep(nhan_vien_bep);

    // Chờ 2 nhân viên làm xong việc mới đóng cửa
    phuc_vu.wait_finish();
    bep.wait_finish();

    std::cout << "\nTổng số đĩa đã chuẩn bị (Atomic): " << dem_so_dia.get() << "\n";
    std::cout << "--- QUÁN ĂN ĐÓNG CỬA ---\n";
    return 0;
}