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
 * Need binpow.
 * O(log m)
 * @param a the number
 * @param m the modulo
 * @return the inverse of a
 */
int modInverse(int a, int m){
    return binpow(a, m-2, m);
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

