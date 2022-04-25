#include <bits/stdc++.h>

using namespace std;

/**
 * Union find with path compression, no union by rank.
 */
struct UnionFind {
    vector<int> parent;
    //initialize with size
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }
    //find parent
    int find(int u) {
        if (parent[u] == u) {
            return u;
        }
        parent[u] = find(parent[u]);
        return parent[u];
    }
    //union nodes
    void unionf(int a, int b) {
        a = find(a);
        b = find(b);
        parent[b] = a;
    }
};

/**
 * Binary Indexed Tree or Fenwick Tree. Kind a useless.
 *
 */
struct BinaryIndexedTree {
    vector<int> bit;
    int n;
    //0 filled tree
    BinaryIndexedTree(int n){
        this->n = n;
        bit.resize(n, 0); //maybe not fill with zeros
    }
    //fill with start vector
    BinaryIndexedTree(vector<int> a) : BinaryIndexedTree(a.size()){
        for (int i = 0; i < a.size(); ++i) {
            update(i, a[i]);
        }
    }
    //To be implemented. Should be reversible
    int func(int a, int b){
        return a+b;
    }
    // calculate func from r to 0
    int calc(int r){
        int ret = 0;//maybe different start value
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

