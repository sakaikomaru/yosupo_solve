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


struct SplayTree {
  struct Node {
    int ch[2] = {0, 0};
    int p = 0;
    Int self = 0, path = 0;
    Int sub = 0, vir = 0;
    bool flip = 0;
  };

  vector<Node> data;

  SplayTree(int n) : data(n + 1) {}

  void push(int x) {
    if (!x || !data[x].flip) return;
    int l = data[x].ch[0];
    int r = data[x].ch[1];
    data[l].flip ^= 1;
    data[r].flip ^= 1;
    swap(data[x].ch[0], data[x].ch[1]);
    data[x].flip = 0;
  }
  void pull(int x) {
    int l = data[x].ch[0];
    int r = data[x].ch[1];
    push(l);
    push(r); 
    data[x].path = data[l].path + data[x].self + data[r].path;
    data[x].sub = data[x].vir + data[l].sub + data[r].sub + data[x].self;
  }
  void set(int x, int d, int y) {
    data[x].ch[d] = y;
    data[y].p = x;
    pull(x); 
  }
  void splay(int x) { 
    auto dir = [&](int x) {
      int p = data[x].p;
      if (!p) return -1;
      return data[p].ch[0] == x ? 0 : data[p].ch[1] == x ? 1 : -1;
    };
    auto rotate = [&](int x) {
      int y = data[x].p;
      int z = data[y].p;
      int dx = dir(x);
      int dy = dir(y);
      set(y, dx, data[x].ch[!dx]); 
      set(x, !dx, y);
      if (~dy) set(z, dy, x); 
      data[x].p = z;
    };
    for (push(x); ~dir(x);) {
      int y = data[x].p;
      int z = data[y].p;
      push(z);
      push(y);
      push(x);
      int dx = dir(x);
      int dy = dir(y);
      if (~dy) rotate(dx != dy ? x : y);
      rotate(x);
    }
  }

};

struct LinkCutTree : SplayTree {
  LinkCutTree(int n) : SplayTree(n) {}
  int access(int x) {
    int u = x;
    int v = 0;
    for (; u; v = u, u = data[u].p) {
      splay(u);
      int& ov = data[u].ch[1];
      data[u].vir += data[ov].sub;
      data[u].vir -= data[v].sub;
      ov = v;
      pull(u);
    }
    return splay(x), v;
  }
  void reroot(int x) {
    access(x);
    data[x].flip ^= 1;
    push(x);
  }
  void Link(int u, int v) { 
    reroot(u);
    access(v); 
    data[v].vir += data[u].sub;
    data[u].p = v;
    pull(v);
  }
  void Cut(int u, int v) {
    reroot(u);
    access(v);
    data[v].ch[0] = data[u].p = 0;
    pull(v);
  }
  int LCA(int u, int v) { 
    if (u == v) return u;
    access(u);
    int ret = access(v); 
    return data[u].p ? ret : 0;
  }
  Int Subtree(int u, int v) {
    reroot(v);
    access(u);
    return data[u].vir + data[u].self;
  }
  Int Path(int u, int v) {
    reroot(u);
    access(v);
    return data[v].path;
  }
  void Update(int u, Int v) {
    access(u);
    data[u].self += v;
    pull(u);
  }
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  LinkCutTree lct(N);
  for (int i = 1; i <= N; i++) {
    cin >> lct.data[i].self;
    lct.pull(i);
  }

  for (int i = 1; i < N; i++) {
    int a, b; cin >> a >> b;
    lct.Link(++a, ++b);
  }

  LOOP(Q) {
    int t; cin >> t;
    if (t == 0) {
      int u, v, w, x; cin >> u >> v >> w >> x;
      lct.Cut(++u, ++v);
      lct.Link(++w, ++x);
    }
    if (t == 1) {
      int p, x; cin >> p >> x;
      lct.Update(++p, x);
    }
    if (t == 2) {
      int u, v; cin >> u >> v;
      cout << lct.Subtree(++u, ++v) << '\n';
    }
  }

  return 0;
}