#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

class EbookManager {
public:
    // нулевые идентификаторы пользователя и номера страниц не используются
    explicit EbookManager() :
        users_progress_(MAX_USER_COUNT + 1, -1),
        pages_readings_(MAX_PAGES_COUNT + 1, 0) {
    }
    
    void Read(int user_id, int page) const {
        UpdatePagesReadings(users_progress_[user_id] + 1, page);
        users_progress_[user_id] = page;
    }

    double Cheer(int user_id) const {
        if(users_progress_[user_id] == -1) {
            return 0;
        }
        const int user_count = GetActiveUserCount();
        return user_count == 1 ? 1 : 
            1.0 - (pages_readings_[users_progress_[user_id]] - 1.0) 
            / (user_count - 1.0);
    }

private:
    const size_t MAX_USER_COUNT = 100000;
    const size_t MAX_PAGES_COUNT = 1000;

    mutable std::vector<int> users_progress_;
    mutable std::vector<int> pages_readings_;

    void UpdatePagesReadings(int from_page, int to_page) const {
        for(int i = from_page; i <= to_page; ++i) {
            ++pages_readings_[i];
        }
    }

    int GetActiveUserCount() const {
        return pages_readings_[0];
    }
};

void ManageEBook(EbookManager& ebook, std::istream& input, std::ostream& output) {
    using namespace std::literals;

    int count;
    input >> count;
    for(int i = 0; i < count; ++ i) {
        std::string querry;
        int user_id;
        input >> querry >> user_id; 
        if(querry == "CHEER"s) {
            output << std::setprecision(6) << ebook.Cheer(user_id) << std::endl;
        } else if(querry == "READ"s) {
            int page;
            input >> page;
            ebook.Read(user_id, page);
        }
    }
}

int main() {
    EbookManager ebook;

    std::istringstream str_stream{R"(12
        CHEER 5
        READ 1 10
        CHEER 1
        READ 2 5
        READ 3 7
        CHEER 2
        CHEER 3
        READ 3 10
        CHEER 3
        READ 3 11
        CHEER 3
        CHEER 1)"
    };

    ManageEBook(ebook, str_stream, std::cout);
}