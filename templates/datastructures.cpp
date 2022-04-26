#include <bits/stdc++.h>

using namespace std;

/**
 * Union find with path compression, no union by rank.
 * find and union in O(almost 1).
 */
struct UnionFind {
    vector<int> parent;
    // initialize with size
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    // find parent
    int find(int u) {
        if (parent[u] == u) {
            return u;
        }
        parent[u] = find(parent[u]);
        return parent[u];
    }
    // union nodes
    void unionf(int a, int b) {
        a = find(a);
        b = find(b);
        parent[b] = a;
    }
};

/**
 * Binary Indexed Tree or Fenwick Tree. Kinda useless, use segment tree instead.
 * Update value and query interval in O(log n).
 */
struct BinaryIndexedTree {
    vector<int> bit;
    int n;
    // 0 filled tree
    BinaryIndexedTree(int n){
        this->n = n;
        bit.resize(n, 0); //maybe not fill with zeros
    }
    // fill with start vector
    BinaryIndexedTree(vector<int> a) : BinaryIndexedTree(a.size()){
        for (int i = 0; i < a.size(); ++i) {
            update(i, a[i]);
        }
    }
    // To be implemented. Should be reversible
    int func(int a, int b){
        return a+b;
    }
    // calculate func from r to 0
    int query(int r){
        int ret = 0;// maybe different start value
        while (r>= 0){
            ret = func(ret, bit[r]);
            r = (r & (r + 1)) -1;
        }
        return ret;
    }
    // update value
    void update(int idx, int delta){
        while (idx<n){
            bit[idx] = func(bit[idx], delta);
            idx|= idx +1;
        }
    }
    // update Range !!!using this allows only point querries
    void updateRange(int l, int r, int delta){
        update(l, delta);
        update(r+1, -delta);
    }
};


/**
 * Mo's algorithm. Answer offline queries in O(n * sqrt(n))
 * Might need to implement data structure very efficiently.
 */
int blocksize; // set blocksize, optimally to sqrt(n)

struct Query {
    int l,r,idx;
    bool operator<(Query other) const {
        return make_pair(l/blocksize, r) < make_pair(other.l / blocksize , other.r);
    }
};

struct Mo {
    vector<int> elements;
    int value;
    // declare data structure
    Mo(vector<int> elements){
        this->elements = elements;
        value = 0;
        // initialize structure
    }
    void remove(int idx){
        // implement remove
    }
    void add(int idx){
        // implement add
    }
    int getAnswer(){
        // implement get answer
    }
    vector<int> algorithm(vector<Query> queries) {
        vector<int> answers(queries.size());
        std::sort(queries.begin(), queries.end());

        int curL = 0, curR = -1;
        for(Query q : queries){
            while (curL > q.l){
                curL--;
                add(curL);
            }
            while (curR < q.r){
                curR++;
                add(curR);
            }
            while (curL < q.l){
                remove(curL);
                curL++;
            }
            while (curR > q.r){
                remove(curR);
                curR--;
            }
            answers[q.idx] = getAnswer();
        }
        return answers;
    }
};


