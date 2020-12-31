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


struct SegmentTreeBeats {
  using T = Int;
  const T ide = 0;
  static const T INF = std::numeric_limits<T>::max();
  static const T NINF = std::numeric_limits<T>::min();
  struct Node {
    T sum;
    T gst1, gst2, gcnt;
    T lst1, lst2, lcnt;
    Int len;
    Int add;
    Node(): gst2(NINF), gcnt(1), lst2(INF), lcnt(1), len(1), add(0) {}
  };
  vector<Node> v;
  Int n, h;
  void fix(int k) {
    Node& p = v[k];
    Node& l = v[k * 2 + 0];
    Node& r = v[k * 2 + 1];
    p.sum = l.sum + r.sum;
    if(r.gst1 < l.gst1) {
      p.gst1 = l.gst1;
      p.gcnt = l.gcnt;
      p.gst2 = max(l.gst2, r.gst1);
    }
    else if(l.gst1 < r.gst1) {
      p.gst1 = r.gst1;
      p.gcnt = r.gcnt;
      p.gst2 = max(l.gst1, r.gst2);
    }
    else {
      p.gst1 = l.gst1;
      p.gcnt = l.gcnt + r.gcnt;
      p.gst2 = max(l.gst2, r.gst2);
    }
    if(r.lst1 > l.lst1) {
      p.lst1 = l.lst1;
      p.lcnt = l.lcnt;
      p.lst2 = min(l.lst2, r.lst1);
    }
    else if(l.lst1 > r.lst1) {
      p.lst1 = r.lst1;
      p.lcnt = r.lcnt;
      p.lst2 = min(l.lst1, r.lst2);
    }
    else {
      p.lst1 = l.lst1;
      p.lcnt = l.lcnt + r.lcnt;
      p.lst2 = min(l.lst2, r.lst2);
    }
  }
  SegmentTreeBeats() {}
  SegmentTreeBeats(const VI& vec) {
    n = 1, h = 1;
    while (n < vec.size()) n <<= 1, h++;
    v.resize(2 * n);
    for (Int i = 0; i < vec.size(); i++) {
      v[i + n].sum = vec[i];
      v[i + n].gst1 = vec[i];
      v[i + n].lst1 = vec[i];
    }
    for (Int i = n; i --> 1;) {
      fix(i);
      v[i].len = v[i * 2 + 0].len + v[i * 2 + 1].len;
    }
  }
  void eff_add(int k, T x) {
    auto& p = v[k];
    p.sum  += x * p.len;
    p.gst1 += x;
    p.lst1 += x;
    p.add += x;
    if (p.gst2 != NINF) p.gst2 += x;
    if (p.lst2 !=  INF) p.lst2 += x;
  }
  void eff_chmin(int k, T x) {
    auto& p = v[k];
    p.sum += (x - p.gst1) * p.gcnt;
    if (p.gst1 == p.lst1) p.gst1 = p.lst1 = x;
    else if (p.gst1 == p.lst2) p.gst1 = p.lst2 = x;
    else p.gst1 = x;
  }
  void eff_chmax(int k, T x) {
    auto& p = v[k];
    p.sum += (x - p.lst1) * p.lcnt;
    if (p.lst1 == p.gst1) p.lst1 = p.gst1 = x;
    else if (p.lst1 == p.gst2) p.lst1 = p.gst2 = x;
    else p.lst1 = x;
  }
  void push(int k) {
    auto& p = v[k];
    if (p.add != 0) {
      eff_add(k * 2 + 0, p.add);
      eff_add(k * 2 + 1, p.add);
      p.add = 0;
    }
    if (p.gst1 < v[k * 2 + 0].gst1) eff_chmin(k * 2 + 0, p.gst1);
    if (p.gst1 < v[k * 2 + 1].gst1) eff_chmin(k * 2 + 1, p.gst1);
    if (p.lst1 > v[k * 2 + 0].lst1) eff_chmax(k * 2 + 0, p.lst1);
    if (p.lst1 > v[k * 2 + 1].lst1) eff_chmax(k * 2 + 1, p.lst1);
  }
  void infuse(int k) {
    k = k >> __builtin_ctz(k);
    while (k >>= 1) fix(k);
  }
  void infiltrate(int k) {
    if (k == n << 1) return;
    for (int i = h; i --> 1;) push(k >> i);
  }
  void subtree_chmin(int k, T x) {
    if (v[k].gst1 <= x) return;
    if (v[k].gst2 < x) {
      eff_chmin(k, x);
      return;
    }
    push(k);
    subtree_chmin(k * 2 + 0, x);
    subtree_chmin(k * 2 + 1, x);
    fix(k);
  }
  void subtree_chmax(int k, T x) {
    if (x <= v[k].lst1) return;
    if (x < v[k].lst2) {
      eff_chmax(k, x);
      return;
    }
    push(k);
    subtree_chmax(k * 2 + 0, x);
    subtree_chmax(k * 2 + 1, x);
    fix(k);
  }
  void range_chmin(int a, int b, T x) {
    infiltrate(a + n);
    infiltrate(b + n);
    int l = a + n;
    int r = b + n;
    while (l < r) {
      if (l & 1) subtree_chmin(l++, x);
      if (r & 1) subtree_chmin(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }
  void range_chmax(int a, int b, T x) {
    infiltrate(a + n);
    infiltrate(b + n);
    int l = a + n;
    int r = b + n;
    while (l < r) {
      if (l & 1) subtree_chmax(l++, x);
      if (r & 1) subtree_chmax(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }
  void range_add(int a, int b, T x) {
    infiltrate(a + n);
    infiltrate(b + n);
    int l = a + n;
    int r = b + n;
    while (l < r) {
      if (l & 1) eff_add(l++, x);
      if (r & 1) eff_add(--r, x);
      l >>= 1;
      r >>= 1;
    }
    infuse(a + n);
    infuse(b + n);
  }
  T range_sum(int l, int r) {
    l += n;
    r += n;
    infiltrate(l);
    infiltrate(r);
    T lx = ide;
    T rx = ide;
    while (l < r) {
      if (l & 1) lx = lx + v[l++].sum;
      if (r & 1) rx = v[--r].sum + rx;
      l >>= 1;
      r >>= 1;
    }
    return lx + rx;
  }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  Int N, Q; cin >> N >> Q;
  VI init(N);
  REP(i,N) cin >> init[i];
  SegmentTreeBeats seg(init);
  while (Q--) {
    Int q; cin >> q;
    if (q == 0) {
      Int l, r, b; cin >> l >> r >> b;
      seg.range_chmin(l, r, b);
    } else if (q == 1) {
      Int l, r, b; cin >> l >> r >> b;
      seg.range_chmax(l, r, b);
    } else if (q == 2) {
      Int l, r, b; cin >> l >> r >> b;
      seg.range_add(l, r, b);
    } else {
      Int l, r; cin >> l >> r;
      cout << seg.range_sum(l, r) << '\n';
    }
  }
  return 0;
}