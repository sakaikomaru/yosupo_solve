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


template<class CommutativeMonoid>
class FenwickTree {
public:
  using T = typename CommutativeMonoid::value_type;
  using size_type = uint_fast32_t;
  using checker = function<bool(T)>;
  vector<T> data;
private:
  size_type get_lsb(size_type i) const { return i & (~i + 1); }
public:
  FenwickTree() = default;
  FenwickTree(const FenwickTree&) = default;
  FenwickTree(FenwickTree&&) = default;
  FenwickTree(size_type size) : data(size + 1, T{}) {}
  template<class InputIt>
  FenwickTree(InputIt first, InputIt last) : FenwickTree(distance(first, last)) {
    for(int index = 0; first != last; first++, index++) add(index, *first);
  }
  FenwickTree& operator=(const FenwickTree&) = default;
  FenwickTree& operator=(FenwickTree&&) = default;
  T fold(size_type last) const {
    T acc{};
    while (last) {
      acc = CommutativeMonoid::operation(data[last], acc);
      last -= get_lsb(last);
    }
    return acc;
  }
  T fold(size_type first, size_type last) const {
    T acc{};
    while (first < last) {
      acc = CommutativeMonoid::operation(data[last], acc);
      last -= get_lsb(last);
    }
    while(last < first) {
      acc = CommutativeMonoid::operation(CommutativeMonoid::inverse(data[first]), acc);
      first -= get_lsb(first);
    }
    return acc;
  }
  void update(size_type index, const T& value) { for(++index; index < data.size(); index += get_lsb(index)) data[index] = CommutativeMonoid::operation(data[index], value); }
  void change(size_type index, const T& value) { update(index, CommutativeMonoid::operation(CommutativeMonoid::inverse((*this)[index]), value)); }
  template<class F>
  size_type search(const F& f) {
    if (f(T{})) return 0;
    T acc{};
    size_type i = 0, k = data.size();
    while(k >>= 1) {
      if((i | k) < data.size() && !f(CommutativeMonoid::operation(acc, data[i | k]))) {
        acc = CommutativeMonoid::operation(acc, data[i | k]);
        i |= k;
      }
    }
    return i + 1;
  }
  T operator[](const size_type& k) const { return fold(k, k + 1); };
  bool empty() const { return size() == 0; }
  size_type size() const { return data.size() - 1; }
  void swap(FenwickTree& r) { data.swap(r.data); }
};


template<class T>
struct add_monoid {
  using value_type = T;
  static value_type operation(const value_type& a, const value_type& b) { return a + b; }
  static value_type inverse(const value_type& x) { return -x; }
};

struct Graph {
  struct Vertex { int n, k0, k1, a; };
  struct Edge { int i, n; };
  Graph(int n, int m) : v(n, { -1, -1, -1, 0 }), e(m), bt(n), n(n), m(0), k(0) {}
  void add_edge(int a, int b) {
    e[m] = { b, v[a].n };
    v[a].n = m;
    m++;
  }
  void dfs(int i) {
    v[i].k0 = k;
    bt.update(k++, v[i].a);
    for (int j = v[i].n; j >= 0; j = e[j].n) {
      Edge& o = e[j];
      dfs(o.i);
    }
    v[i].k1 = k;
  }
  void solve(int q) {
    for (int i = 0; i < n; i++) {
      cin >> v[i].a;
    }
    for (int i = 1; i < n; i++) {
      int p;
      cin >> p;
      add_edge(p, i);
    }
    dfs(0);
    for (int _ = 0; _ < q; _++) {
      int t;
      cin >> t;
      if (t == 0) {
        int u, x;
        cin >> u >> x;
        bt.update(v[u].k0, x);
      } else {
        int u;
        cin >> u;
        cout << bt.fold(v[u].k1) - bt.fold(v[u].k0) << '\n';
      }
    }
  }
  vector<Vertex> v;
  vector<Edge> e;
  FenwickTree<add_monoid<Int>> bt;
  int n, m, k;
};



int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  Int N, Q; cin >> N >> Q;

  Graph g(N, N - 1);
  g.solve(Q);

  return 0;
}