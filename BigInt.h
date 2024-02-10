#include <iostream>
#include <vector>
#include <limits.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include<algorithm>

class BigInt {
public:
    // 成员变量
    std::vector<unsigned int> digits; // 用于存放大整数的每一位
    bool sign; // true 表示正数，false 表示负数

    // 构造函数
    BigInt(int value = 0) {
        if (value < 0) {
            sign = false;
            if (value == INT_MIN) {
                // To prevent Undefined Behavior
                long long valueLong = static_cast<long long>(value);
                unsigned int absValue = static_cast<unsigned int>(std::abs(valueLong));
                digits = std::vector<unsigned int>(1, absValue);
                return;
            } 
            value = -value;
            
        } else {
            sign = true;
        }
        digits = std::vector<unsigned int>(1, value); 
    }

    BigInt(std::vector<unsigned int> digits, bool sign) : digits(digits), sign(sign) {}
    
    BigInt(std::string hexStr) {
        if (hexStr[0] == '-'){
            hexStr = hexStr.substr(1);
            sign = false;
        }
        else sign = true; 

        for (int i = hexStr.length(); i > 2; i -= 8) {
            size_t start = std::max(2, i - 8);
            std::string segment = hexStr.substr(start, i - start);
            std::stringstream ss;
            ss << std::hex << segment;
            unsigned int num;
            ss >> num;
            digits.push_back(num); 
        }
    }

    // 重载运算符
    // 加法
    BigInt operator+(const BigInt& other) const {
        size_t longer = std::max(digits.size(), other.digits.size());
        bool carry = false;

        BigInt result = BigInt();
        result.digits.resize(longer, 0);
        for (size_t i = 0; i < longer; i++) {
            uint num1 = i < digits.size() ? digits[i] : 0;
            uint num2 = i < other.digits.size() ? other.digits[i] : 0;
            
             
            uint sum = num1 + carry;
            carry = sum < num1; //overflow
            sum += num2;
            carry |= sum < num2; //overflow
            result.digits[i] = sum;
        }
        if (carry) {
            result.digits.push_back(1);
        }
        return result;
    }

    BigInt sub_geq(const BigInt& num1, const BigInt& num2) const {
        // Assume num1 >= num2
        size_t length = num1.digits.size();
        bool borrow = false;

        BigInt result = BigInt();
        result.digits.resize(length, 0);
        for (size_t i = 0; i < length; i++) {
            uint a = i < num1.digits.size() ? num1.digits[i] : 0;
            uint b = i < num2.digits.size() ? num2.digits[i] : 0;
            uint c = a - borrow;
            borrow = (c > a); //overflow
            uint d = c - b;
            borrow |= (d > c); //overflow
            result.digits[i] = d;
        }
        result.trim();
        return result;
    }

    // 减法
    BigInt operator-(const BigInt& other) const {
       if (*this < other){
              BigInt result = sub_geq(other, *this);
              result.sign = false;
              return result;
         }
        else{
              BigInt result = sub_geq(*this, other);
              result.sign = true;
              return result;
       }
    }

    BigInt operator<<(int shift) const {
        if (shift < 0){
            std::cerr << "Shift must be non-negative" << std::endl;
            throw std::invalid_argument("Shift must be non-negative");
        }
        BigInt result = *this;
        result.digits.insert(result.digits.begin(), shift, 0);
        
        return result;
    }

    BigInt mult_base(const BigInt& num1, const uint& num2) const
    {
        if (num2 == 0 || num1 == BigInt(0)) {
            return BigInt(0);
        }

        uint carry = 0;
        BigInt result = BigInt();
        result.digits.resize(num1.digits.size(), 0);
        for (size_t i = 0; i < num1.digits.size(); i++) {
            unsigned long long product = static_cast<unsigned long long>(num1.digits[i]) * num2 + carry;
            uint lower = static_cast<uint>(product);
            uint upper = static_cast<uint>(product >> 32);
            
            result.digits[i] = lower;
            carry = upper;
        }
        
        if (carry > 0) {
            result.digits.push_back(carry);
        }
        
        return result;
    }
    // 乘法
    BigInt operator*(const BigInt& other) const {
        if (other == BigInt(0) || *this == BigInt(0)) {
            return BigInt(0);
        }
        auto r = BigInt(0);
        
        for (size_t i = 0; i < other.digits.size(); i++) {
            auto temp = mult_base(*this, other.digits[i]) ;
            temp = temp << i;
            r = r + temp;
        }
        
        return r;
    }

 
    unsigned int estimate_d(const BigInt& x, const BigInt& b) const{
        size_t nb = b.digits.size();
        unsigned int y = nb < x.digits.size() ? x.digits[nb] : 0;
        unsigned int z = nb - 1 < x.digits.size() ? x.digits[nb - 1] : 0;
        unsigned long long yz = (static_cast<unsigned long long>(y) << 32) + z;
        unsigned long long temp = yz / b.digits.back();
        assert (temp <= 0xFFFFFFFF);
        unsigned int d = static_cast<unsigned int>(temp);

        auto product = mult_base(b, d);
        if (product > x)
        {
            d--;
            if (product - b > x){
                d--;
            }
        }

        return d;
    }

    // dividend : a
    // divisor : b

    BigInt operator/(const BigInt& other) const {
        if (other == BigInt(0)) {
            std::cerr << "Division by zero" << std::endl;
            throw std::invalid_argument("Division by zero");
        }

        if (*this < other) {
            return BigInt(0);
        }
        
        auto a = *this;
        auto b = other;
        
        auto e = other.digits.back();
        auto f = BigInt();
        // vector to store long division result
        std::vector<unsigned int> q;

        // normalization
        if (other.digits.size() && e < 0x80000000) {
            f = BigInt(static_cast<unsigned long long>(0x100000000) / (e + 1));
            return (a * f) / (b * f);
        }
        
        assert(e >= 0x80000000);

        size_t na = digits.size();
        size_t nb = b.digits.size();
        assert(na >= nb);
        
        size_t n_down = nb; // number of digits brought down in long division
        std::vector<unsigned int> last_nb(digits.end() - nb, digits.end());
        auto x = BigInt(last_nb, true);
        unsigned int d = estimate_d(x, b);
        q.push_back(d);
        x = x - mult_base(b, d);

        while (n_down < na) {
            x.digits.insert(x.digits.begin(), digits[na - n_down - 1]);
            unsigned int d = estimate_d(x, b);
            q.push_back(d);
            x = x - mult_base(b, d);
            n_down++;
        }

        // change to little endian
        std::reverse(q.begin(), q.end());
        auto r = BigInt(q, true);

        r.trim();
        return r;
        
    }

    bool operator<(const BigInt& other) const {
        if (sign != other.sign) {
            return !sign;
        }
        if (digits.size() != other.digits.size()) {
            return (digits.size() < other.digits.size()) ^ !sign;
        }
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return (digits[i] < other.digits[i]) ^ !sign;
            }
        }
        return false;
    }

    bool operator>(const BigInt& other) const {
        if (sign != other.sign) {
            return sign;
        }
        if (digits.size() != other.digits.size()) {
            return (digits.size() > other.digits.size()) ^ !sign;
        }
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return (digits[i] > other.digits[i]) ^ !sign;
            }
        }
        return false;
    }
    
    bool operator<=(const BigInt& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }
    
    bool operator==(const BigInt& other) const {
        if (sign != other.sign || digits.size() != other.digits.size()) {
            return false;
        }
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i]) {
                return false;
            }
        }
        return true;
    }

    bool isZero() const {
        return digits.size() == 1 && digits[0] == 0;
    }
    
    void trim() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& bigint) { 
        auto vec = bigint.digits;
        if (!bigint.sign) {
            out << "- ";
        }
        
        out << "0x";

        for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
            // output in hex
            std::cout << std::hex << *it << " ";
        }
        return out;
    }
};

