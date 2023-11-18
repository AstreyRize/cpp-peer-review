#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Domain {
public:
    explicit Domain(string domain) {
        reverse(domain.begin(), domain.end());
        domain_ = "." + domain;
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool IsSubdomain(const Domain& other) const {
        if (other.domain_.size() > domain_.size()) {
            return false;
        }

        const auto t = std::mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin()).first;
        return t == other.domain_.end()
            && (domain_.size() == other.domain_.size() || domain_[other.domain_.size()] == '.');
    }

    const string& GetReversedDomain() const {
        return domain_;
    }

private:
    string domain_;
};

vector<Domain> ReadDomains(istream& input, size_t count) {
    vector<Domain> domains;
    domains.reserve(count);
    string line;
    while (count-- > 0) {
        getline(input, line);
        domains.emplace_back(line);
    }
    return domains;
}

class DomainChecker {
public:
    explicit DomainChecker(vector<Domain>& forbidden) {
        sort(forbidden.begin(), forbidden.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetReversedDomain() < rhs.GetReversedDomain();
        });

        const auto last = unique(forbidden.begin(), forbidden.end(),
            [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
        });

        forbidden.erase(last, forbidden.end());
        forbidden_ = std::move(forbidden);
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_.begin(), forbidden_.end(), domain,
            [](const Domain& domain, const Domain& banned) {
            return banned.GetReversedDomain() > domain.GetReversedDomain();
        });

        if (it == forbidden_.begin()) {
            return false;
        }

        --it;
        return domain.IsSubdomain(*it);
    }

private:
    vector<Domain> forbidden_;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    Number num;
    istringstream(line) >> num;
    return num;
}

int main() {
    vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    const DomainChecker checker(forbidden_domains);
    const vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
