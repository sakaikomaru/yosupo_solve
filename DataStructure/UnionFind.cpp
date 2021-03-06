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


struct UnionFind {
  vector<int> data;
  UnionFind(int size) : data(size, -1) {}
  bool unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x != y) {
      if (data[y] < data[x]) swap(x, y);
      data[x] += data[y];
      data[y] = x;
    }
    return x != y;
  }
  inline bool equal(int x, int y) { return find(x) == find(y); }
  inline int find(int x) { return data[x] < 0 ? x : data[x] = find(data[x]); }
  inline int size(int x) { return -data[find(x)]; }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  UnionFind uf(N);
  while (Q--) {
    int t, u, v; cin >> t >> u >> v;
    if (t == 0) uf.unite(u,v);
    else {
      if (uf.equal(u,v)) cout << "1\n";
      else cout << "0\n";
    }
  }

  return 0;
}