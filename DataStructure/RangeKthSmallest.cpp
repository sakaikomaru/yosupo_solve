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

template< typename T, int MAXLOG > struct WaveletMatrix {
  size_t length;
  SuccinctIndexableDictionary matrix[MAXLOG];
  int mid[MAXLOG];
  WaveletMatrix() = default;
  WaveletMatrix(vector<T> v) : length(v.size()) {
    vector<T> l(length), r(length);
    for (int level = MAXLOG - 1; level >= 0; level--) {
      matrix[level] = SuccinctIndexableDictionary(length + 1);
      int left = 0, right = 0;
      for (int i = 0; i < length; i++) {
        if (((v[i] >> level) & 1)) {
          matrix[level].set(i);
          r[right++] = v[i];
        }
        else {
          l[left++] = v[i];
        }
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for (int i = 0; i < right; i++) v[left + i] = r[i];
    }
  }
  pair<int,int> succ(bool f, int l, int r, int level) { return { matrix[level].rank(f, l) + mid[level] * f, matrix[level].rank(f, r) + mid[level] * f }; }
  T access(int k) {
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = matrix[level][k];
      if (f) ret |= T(1) << level;
      k = matrix[level].rank(f, k) + mid[level] * f;
    }
    return ret;
  }
  T operator[](const int &k) { return access(k); }
  int rank(const T &x, int r) {
    int l = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) tie(l, r) = succ((x >> level) & 1, l, r, level);
    return r - l;
  }
  T kth_smallest(int l, int r, int k) {
    assert(0 <= k && k < r - l);
    T ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if (f) {
        ret |= T(1) << level;
        k -= cnt;
      }
      tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }
  T kth_largest(int l, int r, int k) { return kth_smallest(l, r, r - l - k - 1); }
  int range_freq(int l, int r, T upper) {
    int ret = 0;
    for (int level = MAXLOG - 1; level >= 0; level--) {
      bool f = ((upper >> level) & 1);
      if(f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }
  int range_freq(int l, int r, T lower, T upper) { return range_freq(l, r, upper) - range_freq(l, r, lower); }
  T prev_value(int l, int r, T upper) {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? T(-1) : kth_smallest(l, r, cnt - 1);
  }
  T next_value(int l, int r, T lower) {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? T(-1) : kth_smallest(l, r, cnt);
  }
};

template<typename T, int MAXLOG> struct CompressedWaveletMatrix {
  WaveletMatrix<int, MAXLOG> mat;
  vector<T> ys;
  explicit CompressedWaveletMatrix(const vector<T>& v) : ys(v) {
    sort(begin(ys), end(ys));
    ys.erase(unique(begin(ys), end(ys)), end(ys));
    vector<int> t(v.size());
    for(int i = 0; i < v.size(); i++) t[i] = get(v[i]);
    mat = WaveletMatrix<int, MAXLOG>(t);
  }
  inline int get(const T& x) { return lower_bound(begin(ys), end(ys), x) - begin(ys); }
  T access(int k) { return ys[mat.access(k)]; }
  T operator[](const int& k) { return access(k); }
  int rank(const T& x, int r) {
    auto pos = get(x);
    if(pos == ys.size() || ys[pos] != x) return 0;
    return mat.rank(pos, r);
  }
  T kth_smallest(int l, int r, int k) { return ys[mat.kth_smallest(l, r, k)]; }
  T kth_largest(int l, int r, int k) { return ys[mat.kth_largest(l, r, k)]; }
  int range_freq(int l, int r, T upper) { return mat.range_freq(l, r, get(upper)); }
  int range_freq(int l, int r, T lower, T upper) { return mat.range_freq(l, r, get(lower), get(upper)); }
  T prev_value(int l, int r, T upper) {
    auto ret = mat.prev_value(l, r, get(upper));
    return ret == -1 ? T(-1) : ys[ret];
  }
  T next_value(int l, int r, T lower) {
    auto ret = mat.next_value(l, r, get(lower));
    return ret == -1 ? T(-1) : ys[ret];
  }
};



int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  vector<int> A(N);
  REP(i,N) cin >> A[i];
  CompressedWaveletMatrix<int,18> mat(A);
  LOOP(Q) {
    int a, b, c; cin >> a >> b >> c;
    cout << mat.kth_smallest(a, b, c) << '\n';
  }

  return 0;
}