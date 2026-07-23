#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// 1. TÁC VỤ AUTO-SAVE (Dùng detach)
// Chạy ngầm liên tục, không bắt người dùng phải dừng gõ phím để chờ
void tac_vu_auto_save() {
    std::cout << "[Auto-Save]: Luồng chạy ngầm đã kích hoạt!\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Giả lập ghi đĩa tốn 2s
    std::cout << "[Auto-Save]: Đã lưu bản nháp thành công vào ổ đĩa!\n";
}

// 2. TÁC VỤ XUẤT FILE (Dùng Pointer + join)
// Cần con trỏ pointer để cập nhật tiến độ (0% -> 100%) trực tiếp ra ngoài màn hình
void tac_vu_xuat_pdf(const std::string* ten_file, int* phan_tram) {
    std::cout << "[Xuất PDF]: Bắt đầu xuất file: " << *ten_file << "\n";
    
    for (int i = 1; i <= 3; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        *phan_tram = i * 33; // Ghi trực tiếp vào biến ở luồng chính qua Con Trỏ
    }
    *phan_tram = 100;
}

int main() {
    std::cout << "--- MỞ ỨNG DỤNG SOẠN THẢO VĂN BẢN ---\n\n";

    // ========================================================
    // TÌNH HUỐNG 1: AUTO-SAVE (Dùng detach + joinable)
    // ========================================================
    std::thread t_autosave(tac_vu_auto_save);

    // Kiểm tra xem luồng có đang hoạt động không trước khi detach
    if (t_autosave.joinable()) {
        t_autosave.detach(); // Tách ra chạy ngầm! Luồng chính tiếp tục chạy ngay lập tức
        std::cout << "[Main]: Người dùng vẫn tiếp tục gõ văn bản bình thường...\n\n";
    }

    // ========================================================
    // TÌNH HUỐNG 2: XUẤT FILE PDF (Dùng Pointer + join)
    // ========================================================
    std::string ten_document = "bao_cao_daily.pdf";
    int tien_do_xuat = 0;

    // Truyền địa chỉ (&ten_document, &tien_do_xuat) dạng CON TRỎ vào luồng
    std::thread t_export(tac_vu_xuat_pdf, &ten_document, &tien_do_xuat);

    // Luồng chính BẮT BUỘC PHẢI ĐỢI (join) xuất file xong mới làm việc tiếp
    if (t_export.joinable()) {
        std::cout << "[Main]: Đang hiện thanh tiến trình chờ xuất PDF...\n";
        t_export.join(); // Đứng chờ t_export chạy xong
    }

    // Lúc này tiến độ đã được luồng con sửa thành 100% thông qua Con Trỏ
    std::cout << "[Main]: Xuất file hoàn tất! Tiến độ: " << tien_do_xuat << "%\n";
    std::cout << "[Main]: Đã mở file " << ten_document << " lên màn hình.\n\n";

    // Chờ 1 chút để thấy thông báo Auto-Save ngầm in ra màn hình
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "--- ĐÓNG ỨNG DỤNG --- \n";
    return 0;
}