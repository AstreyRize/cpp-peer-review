#include <iostream>
#include <vector>

int main() {
    int query_count, user, page;
    std::cin >> query_count;

    std::vector<int> user_pages(100001, 0);
    std::vector<int> pages_read(1001, 0);
    std::string query;
    while (query_count--) {
        std::cin >> query;
        if (query == "READ") {
            std::cin >> user >> page;
            for (int page_index = user_pages[user] + 1; page_index <= page; ++page_index) {
                ++pages_read[page_index];
            }
            user_pages[user] = page;
        } else if (query == "CHEER") {
            std::cin >> user;
            if (user_pages[user] == 0) {
                std::cout << 0.0 << '\n';
            } else {
                const int total_readers = pages_read[1] - 1;
                const int less_readers = pages_read[user_pages[user]] - 1;
                
                if (total_readers == 0) {
                    std::cout << 1.0 << '\n';
                } else {
                    std::cout << static_cast<double>(total_readers - less_readers) / (total_readers) << '\n';
                }
            }
        }
    }

    return 0;
}
