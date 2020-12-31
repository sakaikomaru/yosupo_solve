#include <bits/stdc++.h>
using namespace std;

using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;
using Int128 = __int128_t;

using Word8 = uint8_t;
using Word16 = uint16_t;
using Word32 = uint32_t;
using Word64 = uint64_t;
using Word128 = __uint128_t;

using Int = int_fast64_t;
using Word = uint_fast64_t;

using F32 = float;
using F64 = double;
using F80 = long double;

using VS = vector<string>;
using VVS = vector<vector<string>>;
using VB = vector<bool>;
using VVB = vector<vector<bool>>;
using VI = vector<Int>;
using VW = vector<Word>;
using VVI = vector<vector<Int>>;
using VVW = vector<vector<Word>>;
using PII = pair<Int,Int>;
using PWW = pair<Word,Word>;
using VPII = vector<pair<Int,Int>>;
using VPWW = vector<pair<Word,Word>>;

#define LOOP(n) for(Int _ipiewnsjiw=0; _ipiewnsjiw<(n); _ipiewnsjiw++)
#define REP(i,n) for(Int i=0, i##_len=(n); i<i##_len; ++i)
#define RANGE(i,a,b) for(Int i=(a), i##_len(b); i<=i##_len; ++i)
#define SZ(obj) ((Int)(obj).size())
#define UNIQUE(obj) (obj).erase(unique((obj).begin(),(obj).end()),(obj).end())
#define ALL(obj) (obj).begin(),(obj).end()
#define RALL(obj) (obj).rbegin(),(obj).rend()


template<typename T, int LOG>
struct PersistentArray {
  struct Node {
    T data;
    Node* child[1 << LOG] = {};
    Node() {}
    Node(const T& data) : data(data) {}
  };
  Node* root;
  PersistentArray() : root(nullptr) {}
  T get(Node* t, int k) {
    if (k == 0) return t->data;
    return get(t->child[k & ((1 << LOG) - 1)], k >> LOG);
  }
  T get(const int &k) { return get(root, k); }
  pair<Node*, T*> mutable_get(Node* t, int k) {
    t = t ? new Node(*t) : new Node();
    if (k == 0) return {t, &t->data};
    auto p = mutable_get(t->child[k & ((1 << LOG) - 1)], k >> LOG);
    t->child[k & ((1 << LOG) - 1)] = p.first;
    return {t, p.second};
  }
  T* mutable_get(const int& k) {
    auto ret = mutable_get(root, k);
    root = ret.first;
    return ret.second;
  }
  Node* build(Node* t, const T& data, int k) {
    if (!t) t = new Node();
    if (k == 0) {
      t->data = data;
      return t;
    }
    auto p = build(t->child[k & ((1 << LOG) - 1)], data, k >> LOG);
    t->child[k & ((1 << LOG) - 1)] = p;
    return t;
  }
  void build(const vector<T>& v) {
    root = nullptr;
    for (int i = 0; i < v.size(); i++) {
      root = build(root, v[i], i);
    }
  }
};
struct PersistentUnionFind {
  PersistentArray<int, 3> data;
  PersistentUnionFind() {}
  PersistentUnionFind(int sz) { data.build(vector< int >(sz, -1)); }
  int find(int k) {
    int p = data.get(k);
    return p >= 0 ? find(p) : k;
  }
  int size(int k) { return (-data.get(find(k))); }
  PersistentUnionFind unite(int x, int y) {
    x = find(x);
    y = find(y);
    if(x == y) return *this;
    auto u = data.get(x);
    auto v = data.get(y);
    if(u < v) {
      auto a = data.mutable_get(x); *a += v;
      auto b = data.mutable_get(y); *b = x;
    }
    else {
      auto a = data.mutable_get(y); *a += u;
      auto b = data.mutable_get(x); *b = y;
    }
    return *this;
  }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  vector<PersistentUnionFind> puf(Q + 1);
  puf[0] = PersistentUnionFind(N);
  for (int i = 1; i <= Q; i++) {
    int t, k, u, v; cin >> t >> k >> u >> v;
    k++;
    if (t == 0) {
      puf[i] = puf[k];
      puf[i] = puf[i].unite(u, v);
    }
    else {
      cout << (puf[k].find(u) == puf[k].find(v)) << '\n';
    }
  }
  return 0;
}