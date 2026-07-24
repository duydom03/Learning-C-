#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// vector<int> twoSum(const vector<int>& nums, int target) {
//     // Key: Giá trị của phần tử, Value: Chỉ số (index) của phần tử đó
//     unordered_map<int, int> seen;

//     for (int i = 0; i < nums.size(); ++i) {
//         int complement = target - nums[i];

//         // Tra cứu xem số còn thiếu đã được lưu trước đó chưa
//         if (seen.find(complement) != seen.end()) {
//             return {seen[complement], i}; // Trả về cặp chỉ số {vị trí cũ, vị trí hiện tại}
//         }

//         // Lưu phần tử hiện tại vào map để phục vụ tra cứu cho các số sau
//         seen[nums[i]] = i;
//     }

//     return {}; // Trả về mảng rỗng nếu không tìm thấy 
// }


vector<pair<int, int>> twoSumAll(const vector<int>& nums, int target) {
    unordered_map<int, int> seen;
    vector<pair<int, int>> result;

    for (int i = 0; i < nums.size(); ++i) {
        int complement = target - nums[i];

        // Tra cứu xem số còn thiếu đã được lưu trước đó chưa
        if (seen.find(complement) != seen.end()) {
            result.push_back({seen[complement], i}); // Lưu cặp chỉ số vào kết quả
        }

        // Lưu phần tử hiện tại vào map để phục vụ tra cứu cho các số sau
        seen[nums[i]] = i;
    }

    return result; // Trả về danh sách các cặp
}

int main() {
    // Tối ưu nhập xuất C++
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    // vector<int> result = twoSum(a, k);

    // if (!result.empty()) {
    //     cout << result[0] << " " << result[1] << "\n";
    // }
    
    vector<pair<int, int>> result = twoSumAll(a, k);

if (!result.empty()) {
    for (const auto& p : result) {
        cout << p.first << " " << p.second << "\n";
    }
} else {
    cout << "No pairs found\n";
}
    return 0;
}