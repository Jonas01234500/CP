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


/**
 * Standart Sum SegmentTree. Point updates in O(log n), range queries in O(log n).
 */
struct SegmentTree{
    int n;
    vector<int> tree, start;

    SegmentTree(vector<int> start){
        n = start.size();
        this->start = start;
        tree = vector<int>(4*n);
        build(1, 0, n-1);
    }

private:
    void build(int v, int tl, int tr){
        if(tl==tr){
            tree[v] = start[tl];
        } else {
            int tm = (tl+tr)/2;
            build(v*2, tl, tm);
            build(v*2+1, tm+1, tr);
            tree[v] = tree[v*2] + tree[v*2+1];
        }
    }

    void update(int v, int tl, int tr, int pos, int nev){
        if(tl==tr){
            tree[v] = nev;
        } else {
            int tm = (tl+tr)/2;
            if(pos<=tm){
                update(2*v, tl, tm, pos, nev);
            } else {
                update(2*v+1, tm+1, tr, pos, nev);
            }
            tree[v] = tree[v*2] + tree[v*2+1];
        }
    }

    int sum(int v, int tl, int tr, int l,int r){
        if(l>r){
            return 0;
        }
        if(l==tl && r == tr){
            return tree[v];
        }
        int tm = (tl+tr)/2;
        return sum(2*v, tl, tm, l ,min(r, tm)) + sum(2*v+1, tm+1, tr, max(l, tm+1), r);
    }

public:
    void update(int pos, int nev){
        update(1, 0, n-1, pos, nev);
    }

    int sum(int l, int r){
        return sum(1, 0, n-1, l, r);
    }
};

/**
 * Segment tree with lazy propagation. Range Update an Point Query in O(log n).
 */
struct LazyTree{
    int n;
    vector<int> tree, start;
    vector<bool> marked;

    LazyTree(vector<int> start){
        n = start.size();
        this->start = start;
        tree = vector<int>(4*n);
        marked = vector<bool>(4*n, false);
        build(1, 0, n-1);
    }

private:
    void build(int v, int tl, int tr){
        if(tl==tr){
            tree[v] = start[tl];
        } else {
            int tm = (tl+tr)/2;
            build(v*2, tl, tm);
            build(v*2+1, tm+1, tr);
            tree[v] = tree[v*2] + tree[v*2+1];
        }
    }

    void push(int v) {
        if (marked[v]) {
            tree[v*2] = tree[v*2+1] = tree[v];
            marked[v*2] = marked[v*2+1] = true;
            marked[v] = false;
        }
    }

    void update(int v, int tl, int tr, int l, int r, int val) {
        if (l > r)
            return;
        if (l == tl && tr == r) {
            tree[v] = val;
            marked[v] = true;
        } else {
            push(v);
            int tm = (tl + tr) / 2;
            update(v*2, tl, tm, l, min(r, tm), val);
            update(v*2+1, tm+1, tr, max(l, tm+1), r, val);
        }
    }

    int get(int v, int tl, int tr, int pos) {
        if (tl == tr) {
            return tree[v];
        }
        push(v);
        int tm = (tl + tr) / 2;
        if (pos <= tm)
            return get(v*2, tl, tm, pos);
        else
            return get(v*2+1, tm+1, tr, pos);
    }

public:
    void update(int l, int r, int val){
        update(1, 0, n-1, l, r, val);
    }

    int get(int pos){
        return get(1, 0, n-1, pos);
    }
};


