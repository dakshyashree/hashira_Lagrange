#include <bits/stdc++.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;
using boost::multiprecision::cpp_int;

// --------- Utilities ---------
static inline bool is_all_digits(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit((unsigned char)c)) return false;
    return true;
}

// parse a string in base (2..36) into cpp_int
cpp_int parse_in_base(const string& s_raw, int base) {
    string s = s_raw;
    // normalize to lowercase
    for (char& c : s) c = (char)tolower((unsigned char)c);
    cpp_int val = 0;
    for (char c : s) {
        int d;
        if (c >= '0' && c <= '9') d = c - '0';
        else if (c >= 'a' && c <= 'z') d = 10 + (c - 'a');
        else if (c == '+' || c == '-') {
            throw runtime_error("Only non-negative digits expected in values.");
        } else {
            throw runtime_error("Invalid digit in value: " + string(1, c));
        }
        if (d >= base) throw runtime_error("Digit out of range for base");
        val = val * base + d;
    }
    return val;
}

// gcd for cpp_int
cpp_int big_gcd(cpp_int a, cpp_int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        cpp_int r = a % b;
        a = b; b = r;
    }
    return a;
}

// --------- Exact rational ----------
struct Frac {
    cpp_int num; // numerator
    cpp_int den; // denominator > 0

    Frac() : num(0), den(1) {}
    explicit Frac(const cpp_int& n) : num(n), den(1) {}
    Frac(const cpp_int& n, const cpp_int& d) { set(n, d); }

    void normalize() {
        if (den == 0) throw runtime_error("Zero denominator");
        if (den < 0) { num = -num; den = -den; }
        cpp_int g = big_gcd(num, den);
        if (g != 0) { num /= g; den /= g; }
    }
    void set(const cpp_int& n, const cpp_int& d) {
        num = n; den = d; normalize();
    }

    Frac operator+(const Frac& o) const {
        Frac r(num * o.den + o.num * den, den * o.den);
        r.normalize();
        return r;
    }
    Frac operator*(const Frac& o) const {
        Frac r(num * o.num, den * o.den);
        r.normalize();
        return r;
    }
};

// --------- Main -----------
struct Point { cpp_int x, y; };

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Read JSON from stdin or from file path argument
    string input;
    if (argc >= 2) {
        ifstream fin(argv[1]);
        if (!fin) { cerr << "Failed to open file: " << argv[1] << "\n"; return 1; }
        input.assign((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    } else {
        input.assign((istreambuf_iterator<char>(cin)), istreambuf_iterator<char>());
    }

    json j;
    try { j = json::parse(input); }
    catch (const std::exception& e) {
        cerr << "JSON parse error: " << e.what() << "\n";
        return 1;
    }

    if (!j.contains("keys") || !j["keys"].contains("k")) {
        cerr << "Invalid JSON: missing keys.k\n";
        return 1;
    }

    int k = j["keys"]["k"].get<int>();

    vector<Point> pts;
    for (auto it = j.begin(); it != j.end(); ++it) {
        const string key = it.key();
        if (key == "keys") continue;
        if (!is_all_digits(key)) continue; // only numeric x's

        const json& node = it.value();
        if (!node.contains("base") || !node.contains("value")) continue;

        int base = stoi(node["base"].get<string>());
        string valstr = node["value"].get<string>();
        if (base < 2 || base > 36) {
            cerr << "Unsupported base: " << base << " for key " << key << "\n";
            return 1;
        }

        cpp_int y = parse_in_base(valstr, base);
        cpp_int x = 0; // parse key as integer
        {
            // fast parse decimal key into cpp_int
            for (char c : key) { x *= 10; x += (c - '0'); }
        }
        pts.push_back(Point{x, y});
    }

    if ((int)pts.size() < k) {
        cerr << "Not enough points to interpolate: have " << pts.size() << ", need " << k << "\n";
        return 1;
    }

    // Sort by x and take first k
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){ return a.x < b.x; });
    pts.resize(k);

    // Lagrange interpolation at x = 0:
    // f(0) = sum_i [ y_i * Π_{j!=i} (-x_j)/(x_i - x_j) ]
    Frac f0(cpp_int(0));
    for (int i = 0; i < k; ++i) {
        Frac term(pts[i].y);
        for (int j = 0; j < k; ++j) if (i != j) {
            cpp_int num = -pts[j].x;             // (-x_j)
            cpp_int den =  pts[i].x - pts[j].x;  // (x_i - x_j)
            term = term * Frac(num, den);
        }
        f0 = f0 + term;
    }

    // Must be an integer by construction
    if (f0.den != 1) {
        // As a safety, check divisibility
        cpp_int q = f0.num / f0.den;
        cpp_int r = f0.num % f0.den;
        if (r != 0) {
            cerr << "Result is not an integer: " << f0.num << "/" << f0.den << "\n";
            return 1;
        }
        cout << q << "\n";
    } else {
        cout << f0.num << "\n";
    }

    return 0;
}
