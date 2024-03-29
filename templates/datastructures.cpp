#include <bits/stdc++.h>

using namespace std;

/**
 * Union find with path compression, no union by rank.
 * find and union in O(almost 1).
 */
struct UnionFind {
    vector<int> parent;
    UnionFind(int n) {
        parent.resize(n);
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int u) {
        if (parent[u] == u) return u;
        return parent[u] = find(parent[u]);
    }
    void uniun(int a, int b) {
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

};

/** Implizit Treap, split and merge in O(log n).
 *
 */
struct item {
    int val, prior, cnt;
    item *l, *r;
    item () { }
    item (int val) : val(val), prior(rand()), l(NULL), r(NULL) { }
    item (int val, int prior) : val(val), prior(prior), l(NULL), r(NULL) { }
};
typedef item* pitem;

int cnt (pitem t) {
    return t ? t->cnt : 0;
}

void upd_cnt (pitem t) {
    if (t)
        t->cnt = 1 + cnt(t->l) + cnt (t->r);
}

void merge (pitem & t, pitem l, pitem r) {
    if (!l || !r)
        t = l ? l : r;
    else if (l->prior > r->prior)
        merge (l->r, l->r, r),  t = l;
    else
        merge (r->l, l, r->l),  t = r;
    upd_cnt (t);
}

void split (pitem t, pitem & l, pitem & r, int key, int add = 0) {
    if (!t)
        return void( l = r = 0 );
    int cur_key = add + cnt(t->l); //implicit key
    if (key <= cur_key)
        split (t->l, l, t->l, key, add),  r = t;
    else
        split (t->r, t->r, r, key, add + 1 + cnt(t->l)),  l = t;
    upd_cnt (t);
}


/**
 * STL-Tree
 */
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std; using namespace __gnu_pbds;
template<typename T>
using Tree = tree<T, null_type, less<T>, rb_tree_tag,tree_order_statistics_node_update>;

