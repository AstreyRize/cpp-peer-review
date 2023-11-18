#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <sstream>

enum class DBLogLevel { Debug, Info, Warn, Error };
class DBConnector { /*...*/ };
class DBQuery { /*...*/ };
class DBHandler {
public:
    bool IsOK() const { /*...*/ }
    void Disconnect() { /*...*/ }
    std::string Quote(const std::string& str) const { /*...*/ }
};
struct Person {
    std::string name;
    int age;
};

struct DBConfig {
    std::string db_name;
    int connection_timeout;
    bool allow_exceptions;
    DBLogLevel log_level;

    DBConfig(const std::string& name, int timeout, bool allow_exc, DBLogLevel level)
        : db_name(name), connection_timeout(timeout), allow_exceptions(allow_exc), log_level(level) {}
};

class Database {
public:
    explicit Database(const DBConfig& config)
        : config_(config) {}

    std::optional<DBHandler> Connect() {
        DBConnector connector(config_.allow_exceptions, config_.log_level);
        db_ = config_.db_name.starts_with("tmp."s) ?
            connector.ConnectTmp(config_.db_name, config_.connection_timeout) :
            connector.Connect(config_.db_name, config_.connection_timeout);

        if (!config_.allow_exceptions && (!db_ || !db_->IsOK())) {
            return std::nullopt;
        }
        return db_;
    }

    ~Database() {
        if (db_ && db_->IsOK()) {
            db_->Disconnect();
        }
    }

private:
    DBConfig config_;
    std::optional<DBHandler> db_;
};

struct LoadPersonsQuery {
    int min_age;
    int max_age;
    std::string name_filter;

    LoadPersonsQuery(int min_age, int max_age, std::string name_filter)
        : min_age(min_age), max_age(max_age), name_filter(std::move(name_filter)) {}
};

std::optional<std::vector<Person>> LoadPersons(Database& db, const LoadPersonsQuery& query) {
    using namespace std::literals::string_literals;

    auto db_handler = db.Connect();
    if (!db_handler) {
        return std::nullopt;
    }

    std::ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << query.min_age << " and "s << query.max_age << " "s
        << "and Name like '%"s << db_handler->Quote(query.name_filter) << "%'"s;
    DBQuery dbQuery(query_str.str());

    std::vector<Person> persons;
    for (auto [name, age] : db_handler->LoadRows<std::string, int>(dbQuery)) {
        persons.push_back({ std::move(name), age });
    }

    return persons;
}

int main() {
    using namespace std;

    try {
        const DBConfig db_config("tmp.db"s, 30, true, DBLogLevel::Debug);
        const Database db(db_config);

        const LoadPersonsQuery query(16, 24, "John");
        auto persons = LoadPersons(db, query);
        if (persons) {
            for (const auto& person : persons.value()) {
                std::cout << person.name << " " << person.age << std::endl;
            }
        } else {
            std::cout << "No persons found" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
