#include <bits/stdc++.h>
using namespace std;

/**
 * Power with modulo
 * O(log b)
 * @param a base
 * @param b exponent
 * @param m modulo
 * @return a^b mod m
 */
int binpow(int a, int b, int m) {
    a %= m;
    int res = 1;
    while (b > 0) {
        if (b & 1) {
            res = res * a % m;
        }
        a = a * a % m;
        b >>= 1;
    }
    return res;
}

/**
 * Calculate modular inverse.
 * O(log m)
 * @param a the number
 * @param m the modulo
 * @return the inverse of a
 */
int inv(int a, int b){
    return 1<a ? b - inv(b%a,a)*b/a : 1;
}

/**
 * Get random normal distributed number from a to b.
 */
std::random_device dev;
std::mt19937 rng(dev());
int ran(int a,int b){
    uniform_int_distribution<std::mt19937::result_type> dist(a,b);
    return dist(rng);
}

/**
 * Calculate discrete logarithm (a^x = b mod m).
 * Need binpow.
 * O(sqrt(m) * log m)
 * @param a base, should be coprime to m
 * @param b result
 * @param m modulo, should be coprime to a
 * @return -1, if no solution exists, x otherwise
 */
int disLog(int a, int b, int m) {
    a %= m, b %= m;
    int n = sqrt(m) + 1;
    map<int, int> vals;
    for (int p = 1; p <= n; ++p)
        vals[binpow(a, p * n, m)] = p;
    for (int q = 0; q <= n; ++q) {
        int cur = (binpow(a, q, m) * 1ll * b) % m;
        if (vals.count(cur)) {
            int ans = vals[cur] * n - q;
            return ans;
        }
    }
    return -1;
}


/**
 * Calculate Primes. Fills vector primes with all primes up to n
 * O(n log log n)
 */
vector<int> primes;
void calcPrimes(int n){
    vector<bool> isPrime(n+1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i)
                isPrime[j] = false;
        }
    }
    for (int i = 0; i < isPrime.size(); ++i) {
        if(isPrime[i]){
            primes.push_back(i);
        }
    }
}
/**
 * Big Integer for posive numbers. Don't substract larger numbers!. Everything works in O(length of number) except multilplikation is O(n^log2(3);
 */
struct BigInt {
    vector<int> number;
    int base = 1e9;

    string to_string() {
        string s = number.empty() ? "0" : ::to_string(number.back());
        for (int i = (int) number.size() - 2; i >= 0; --i) {
            string ts = ::to_string(number[i]);
            s += string(9 - ts.size(), '0');
            s += ts;
        }
        return s;
    }

    BigInt(string s) {
        for (int i = (int) s.length(); i > 0; i -= 9) {
            if (i < 9)
                number.push_back(atoi(s.substr(0, i).c_str()));
            else
                number.push_back(atoi(s.substr(i - 9, 9).c_str()));
        }
        while (number.size() > 0 && number.back() == 0) {
            number.pop_back();
        }
    }

    BigInt() {

    }

    BigInt operator+(BigInt o) {
        BigInt res;
        int carry = 0;
        for (int i = 0; i < min(o.number.size(), number.size()); ++i) {
            res.number.push_back(carry + number[i] + o.number[i]);
            carry = res.number[i] / base;
            res.number[i] %= base;
        }
        if (number.size() >= o.number.size()) {
            for (int i = o.number.size(); i < number.size(); ++i) {
                res.number.push_back(carry + number[i]);
                carry = res.number[i] / base;
                res.number[i] %= base;
            }
        } else {
            for (int i = number.size(); i < o.number.size(); ++i) {
                res.number.push_back(carry + o.number[i]);
                carry = res.number[i] / base;
                res.number[i] %= base;
            }
        }
        if (carry > 0) {
            res.number.push_back(carry);
        }
        while (res.number.size() > 0 && res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    BigInt operator-(BigInt o) {
        BigInt res;
        int carry = 0;
        for (int i = 0; i < o.number.size(); ++i) {
            if (number[i] >= o.number[i] + carry) {
                res.number.push_back(number[i] - o.number[i] - carry);
                carry = 0;
            } else {
                res.number.push_back(base + number[i] - o.number[i] - carry);
                carry = 1;
            }
        }
        while (res.number.size() > 0 && res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }

    BigInt operator*(BigInt o){
        BigInt res;
        if(number.size()==0 ||o.number.size()==0){
            return res;
        }
        if(number.size()==1){
            return o.multiply(number[0]);
        }
        if(o.number.size()==1){
            return multiply(o.number[0]);
        }
        int m = min(number.size(), o.number.size())/2;
        BigInt high1, high2, low1,low2;
        for (int i = 0; i < number.size(); ++i) {
            if(i<m){
                low1.number.push_back(number[i]);
            } else {
                high1.number.push_back(number[i]);
            }
        }
        for (int i = 0; i < o.number.size(); ++i) {
            if(i<m){
                low2.number.push_back(o.number[i]);
            } else {
                high2.number.push_back(o.number[i]);
            }
        }
        BigInt z0, z1, z2;
        z0 = low1*low2;
        z1 = (low1+high1) * (low2+high2);
        z2 = high1*high2;
        BigInt v1,v2;
        for (int i = 0; i < 2*m; ++i) {
            v1.number.push_back(0);
        }
        for (int i = 0; i < z2.number.size(); ++i) {
            v1.number.push_back(z2.number[i]);
        }
        for (int i = 0; i < m; ++i) {
            v2.number.push_back(0);
        }
        BigInt tmp = z1-z2-z0;
        for (int i = 0; i < tmp.number.size(); ++i) {
            v2.number.push_back(tmp.number[i]);
        }
        return v1 + v2 + z0;

    }


    bool operator<(BigInt o) {
        if (number.size() == o.number.size()) {
            for (int i = number.size() - 1; i >= 0; --i) {
                if (number[i] != o.number[i]) {
                    return number[i] < o.number[i];
                }
            }
        }
        return number.size() < o.number.size();
    }

    bool operator==(BigInt o) {
        if (number.size() == o.number.size()) {
            for (int i = number.size() - 1; i >= 0; --i) {
                if (number[i] != o.number[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    bool operator>(BigInt o) {
        return !(*this == o || *this < o);
    }

    BigInt multiply(int i) {
        BigInt res;
        int carry = 0;
        for (int j = 0; j < number.size(); ++j) {
            res.number.push_back(number[j] * i + carry);
            carry = res.number[j] / base;
            res.number[j] %= base;
        }
        if (carry > 0) {
            res.number.push_back(carry);
        }
        return res;
    }

    BigInt divide(int i){
        BigInt res;
        res.number = vector<int>(number.size());
        int carry = 0;
        for (int j=(int)number.size()-1; j>=0; --j) {
            int cur = number[j] + carry * base;
            res.number[j] = (cur/i);
            carry = cur%i;
        }
        while (res.number.size() > 0 && res.number.back() == 0) {
            res.number.pop_back();
        }
        return res;
    }
};