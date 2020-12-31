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

template<typename T>
struct BinaryIndexedTree {
  vector<T> data;
  BinaryIndexedTree() = default;
  explicit BinaryIndexedTree(size_t sz) : data(sz + 1, 0) {}
  explicit BinaryIndexedTree(const vector<T>& vs) : data(vs.size() + 1, 0) {
    for (size_t i = 0; i < vs.size(); i++) data[i + 1] = vs[i];
    for (size_t i = 1; i < data.size(); i++) {
      size_t j = i + (i & -i);
      if (j < data.size()) data[j] += data[i];
    }
  }
  void add(int k, const T &x) { for (++k; k < (int) data.size(); k += k & -k) data[k] += x; }
  T query(int k) const {
    T ret = T();
    for (++k; k > 0; k -= k & -k) ret += data[k];
    return ret;
  }
  int lower_bound(T x) const {
    int i = 0;
    for (int k = 1 << (__lg(data.size() - 1) + 1); k > 0; k >>= 1) {
      if (i + k < data.size() && data[i + k] < x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }
  int upper_bound(T x) const {
    int i = 0;
    for (int k = 1 << (__lg(data.size() - 1) + 1); k > 0; k >>= 1) {
      if (i + k < data.size() && data[i + k] <= x) {
        x -= data[i + k];
        i += k;
      }
    }
    return i;
  }
};

struct SuccinctIndexableDictionary {
  size_t length;
  size_t blocks;
  vector<unsigned> bit, sum;
  SuccinctIndexableDictionary() = default;
  SuccinctIndexableDictionary(size_t length) : length(length), blocks((length + 31) >> 5) {
    bit.assign(blocks, 0U);
    sum.assign(blocks, 0U);
  }
  void set(int k) { bit[k >> 5] |= 1U << (k & 31); }
  void build() {
    sum[0] = 0U;
    for(int i = 1; i < blocks; i++) sum[i] = sum[i - 1] + __builtin_popcount(bit[i - 1]);
  }
  bool operator[](int k) { return (bool((bit[k >> 5] >> (k & 31)) & 1)); }
  int rank(int k) { return (sum[k >> 5] + __builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1))); }
  int rank(bool val, int k) { return (val ? rank(k) : k - rank(k)); }
};

template<typename T, int MAXLOG, typename D> struct WaveletMatrixRectangleSum {
  size_t length;
  SuccinctIndexableDictionary matrix[MAXLOG];
  BinaryIndexedTree<D> ds[MAXLOG];
  int mid[MAXLOG];
  WaveletMatrixRectangleSum() = default;
  WaveletMatrixRectangleSum(const vector<T>& v, const vector<D>& d) : length(v.size()) {
    assert(v.size() == d.size());
    vector<int> l(length), r(length), ord(length);
    iota(begin(ord), end(ord), 0);
    vector<D> dd(length);
    for (int level = MAXLOG - 1; level >= 0; level--) {
      matrix[level] = SuccinctIndexableDictionary(length + 1);
      int left = 0, right = 0;
      for (int i = 0; i < length; i++) {
        if (((v[ord[i]] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = ord[i];
        }
        else l[left++] = ord[i];
      }
      mid[level] = left;
      matrix[level].build();
      ord.swap(l);
      for (int i = 0; i < right; i++) ord[left + i] = r[i];
      for (int i = 0; i < length; i++) dd[i] = d[ord[i]];
      ds[level] = BinaryIndexedTree<D>(dd);
    }
  }
  pair<int, int> succ(bool f, int l, int r, int level) { return {matrix[level].rank(f, l) + mid[level] * f, matrix[level].rank(f, r) + mid[level] * f}; }
  D rect_sum(int l, int r, T upper) {
    D ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = ((upper >> level) & 1);
      if (f) ret += ds[level].query(matrix[level].rank(false, r) - 1) - ds[level].query(matrix[level].rank(false, l) - 1);
      tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }
  D rect_sum(int l, int r, T lower, T upper) { return rect_sum(l, r, upper) - rect_sum(l, r, lower); }
  void point_add(int k, T y, const D& x) {
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = ((y >> level) & 1);
      k = matrix[level].rank(f, k) + mid[level] * f;
      ds[level].add(k, x);
    }
  }
};

template<typename T, int MAXLOG, typename D> struct CompressedWaveletMatrixRectangleSum {
  WaveletMatrixRectangleSum<int, MAXLOG, D> mat;
  vector<T> ys;
  CompressedWaveletMatrixRectangleSum(const vector<T>& v, const vector<D>& d) : ys(v) {
    sort(begin(ys), end(ys));
    ys.erase(unique(begin(ys), end(ys)), end(ys));
    vector<int> t(v.size());
    for (int i = 0; i < v.size(); i++) t[i] = get(v[i]);
    mat = WaveletMatrixRectangleSum<int, MAXLOG, D>(t, d);
  }
  inline int get(const T &x) { return lower_bound(begin(ys), end(ys), x) - begin(ys); }
  D rect_sum(int l, int r, T upper) { return mat.rect_sum(l, r, get(upper)); }
  D rect_sum(int l, int r, T lower, T upper) { return mat.rect_sum(l, r, get(lower), get(upper)); }
  void point_add(int k, T y, const D &x) { mat.point_add(k, get(y), x); }
};



int main() {
  int N, Q; cin >> N >> Q;
  vector<int> x(N), y(N), w(N);
  vector<pair<int, int>> xs;
  xs.reserve(N + Q);
  REP(i,N) {
    cin >> x[i] >> y[i] >> w[i];
    xs.emplace_back(x[i],y[i]);
  }
  vector<int> t(Q), a(Q), b(Q), c(Q), d(Q);
  REP(i,Q) {
    cin >> t[i] >> a[i] >> b[i] >> c[i];
    if (t[i] == 0) xs.emplace_back(a[i], b[i]);
    else cin >> d[i];
  }
  sort(begin(xs), end(xs));
  xs.erase(unique(begin(xs), end(xs)), end(xs));
  vector<int> ys(xs.size());
  for (int i = 0; i < xs.size(); i++) ys[i] = xs[i].second;
  vector<Int> ws(xs.size());
  REP(i,N) {
    x[i] = lower_bound(begin(xs),end(xs),make_pair(x[i], y[i])) - begin(xs);
    ws[x[i]] += w[i];
  }
  REP(i,Q) {
    if (t[i] == 0) a[i] = lower_bound(begin(xs), end(xs), make_pair(a[i], b[i])) - begin(xs);
    else {
      a[i] = lower_bound(begin(xs), end(xs), make_pair(a[i], -1)) - begin(xs);
      c[i] = lower_bound(begin(xs), end(xs), make_pair(c[i], -1)) - begin(xs);
    }
  }
  CompressedWaveletMatrixRectangleSum<int, 18, Int> mat(ys, ws);
  REP(i,Q) {
    if (t[i] == 0) mat.point_add(a[i], b[i], c[i]);
    else cout << (mat.rect_sum(a[i], c[i], b[i], d[i])) << '\n';
  }
  return 0;
}