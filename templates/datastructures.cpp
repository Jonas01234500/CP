#include <bits/stdc++.h>
using namespace std;

/**
 * Union find with path compression, no union by rank.
 * Initialize with size.
 */
vector<int> parent;
void initializeUF(int n){
    parent.resize(n);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int find(int u){
    if(parent[u] == u){
        return u;
    }
    parent[u] = find(parent[u]);
    return parent[u];
}

void unionf(int a, int b){
    a = find(a);
    b = find(b);
    parent[b] = a;
}
