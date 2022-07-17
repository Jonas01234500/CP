#include <bits/stdc++.h>

using namespace std;
#define int long long
#define double long double
#define ii pair<int, int>
#define ui uint64_t
#define nl "\n"
#define yes "YES\n"
#define no "NO\n"
#define inv vector<int> v(n);for(int i = 0; i<(n);++i){cin >> v[i];}
#define st ios_base::sync_with_stdio(false);cin.tie(nullptr);cout.tie(nullptr)
#define fi first
#define si second
#define tsolve() int t; cin >> t;while(t--){solve();}
#define print(b) if(b){cout << "YES\n";}else {cout << "NO\n";}

int mo = 1020202009;


void solve() {
    int n,s;
    cin >> n >> s;
    set<ii, greater<>> a,b;
    int hap = 0,as=0,bs=0;
    int zero = 0;
    for (int i = 0; i < n; ++i) {
        int sl,ai,bi;
        cin >> sl >> ai >> bi;
        hap+=min(ai,bi)*sl;
        if(ai>bi){
            a.insert({ai-bi, sl});
            as+=sl;
        }
        if(bi>ai){
            b.insert({bi-ai,sl});
            bs+=sl;
        }
        if(bi==ai){
            zero+=sl;
        }
    }
    while (as>=s){
        int k = s;
        if(a.begin()->si<=s){
            while (k>0 && a.begin()->si<=k){
                hap+=a.begin()->si*a.begin()->fi;
                k-=a.begin()->si;
                as-=a.begin()->second;
                a.erase(a.begin());
            }
        } else {
            k = a.begin()->second/s*s;
        }
        if(k!=0){
            ii ne = *a.begin();
            ne.si-=k;
            as-=k;
            hap+=ne.fi*k;
            a.erase(a.begin());
            a.insert(ne);
        }
    }
    while (bs>=s){
        int k = s;
        if(b.begin()->si<=s){
            while (k>0 && b.begin()->si<=k){
                hap+=b.begin()->si*b.begin()->fi;
                k-=b.begin()->si;
                bs-=b.begin()->second;
                b.erase(b.begin());
            }
        } else {
            k = b.begin()->second/s*s;
        }
        if(k!=0) {
            ii ne = *b.begin();
            ne.si -= k;
            bs -= k;
            hap += ne.fi * k;
            b.erase(b.begin());
            b.insert(ne);
        }
    }
    int ah=0,bh=0;
    for (auto it: a) {
        ah+=it.second*it.fi;
    }
    for (auto it: b) {
        bh+=it.second*it.fi;
    }
    if(zero+as+bs>s){
        hap+= as+bs;
    } else {
        hap+= max(ah,bh);
    }
    cout << hap;
}

int32_t main() {
    st;
    solve();
}