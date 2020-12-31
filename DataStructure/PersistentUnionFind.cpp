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


struct UnionFindUndo {
  vector<int> dat;
  stack<pair<int,int>> hist;
  UnionFindUndo(int n) : dat(n, -1) {}
  int root(int x) { return (dat[x] < 0 ? x : root(dat[x])); }
  bool unite(int x, int y) {
    x = root(x);
    y = root(y);
    hist.emplace(x, dat[x]);
    hist.emplace(y, dat[y]);
    if (x == y) return false;
    if (dat[x] > dat[y]) swap(x, y);
    dat[x] += dat[y];
    dat[y] = x;
    return true;
  }
  void undo() {
    dat[hist.top().first] = hist.top().second; hist.pop();
    dat[hist.top().first] = hist.top().second; hist.pop();
  }
  void snapshot() { while(!hist.empty()) hist.pop(); }
  void rollback() { while(!hist.empty()) undo(); }
  bool same(int x,int y) { return root(x) == root(y); }
  int size(int x) { return -dat[root(x)]; }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  vector<int> t(Q), u(Q), v(Q);
  vector<vector<int>> G(Q + 1);
  for (int i = 0; i < Q; i++) {
    int k; cin >> t[i] >> k >> u[i] >> v[i];
    G[++k].emplace_back(i + 1);
  }
  UnionFindUndo uf(N);
  vector<int> ans(Q, -1);
  auto dfs = [&](auto self, int cur) -> void {
    if (cur) {
      if (!t[cur - 1]) uf.unite(u[cur - 1], v[cur - 1]);
      else ans[cur - 1] = uf.same(u[cur - 1], v[cur - 1]);
    }
    for (int nxt : G[cur]) self(self, nxt);
    if (cur && !t[cur - 1]) uf.undo();
  };
  dfs(dfs, 0);
  for (int i = 0; i < Q; i++) if (~ans[i]) cout << ans[i] << '\n';
  return 0;
}