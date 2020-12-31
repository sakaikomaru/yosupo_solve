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


template<typename Comp>
struct LinearRMQ {
  static constexpr int BLOCKSIZE = 32;
  using block_type = uint_least32_t;
  vector<block_type> small;
  vector<vector<int>> large;
  LinearRMQ() = default;
  Comp comp;
  static inline int msb(int c) { return 31 - __builtin_clz(c); }
  static inline int ctz(int c) { return __builtin_ctz(c); }
  inline int get_min(int l, int r) const { return comp(l, r) ? l : r; }
  explicit LinearRMQ(size_t n, const Comp& comp) : comp(comp) {
    vector<int> st;
    st.reserve(BLOCKSIZE);
    large.emplace_back();
    large.front().reserve(n / BLOCKSIZE);
    small.reserve(n);
    for (int i = 0; i < n; i++) {
      while (!st.empty() && !comp(st.back(), i)) st.pop_back();
      small.emplace_back(st.empty() ? 0 : small[st.back()]);
      small.back() |= static_cast<block_type>(1) << (i % BLOCKSIZE);
      st.emplace_back(i);
      if ((i + 1) % BLOCKSIZE == 0) {
        large.front().emplace_back(st.front());
        st.clear();
      }
    }
    for (int i = 1; (i << 1) <= n / BLOCKSIZE; i <<= 1) {
      vector<int> v;
      int csz = n / BLOCKSIZE + 1 - (i << 1);
      v.reserve(csz);
      for(int k = 0; k < csz; k++) v.emplace_back(get_min(large.back()[k], large.back()[k + i]));
      large.emplace_back(move(v));
    }
  }
  int fold(int l, int r) const {
    --r;
    int left = l / BLOCKSIZE + 1;
    int right = r / BLOCKSIZE;
    if(left < right) {
      auto p = msb(right - left);
      return get_min( get_min((left - 1) * BLOCKSIZE + ctz(small[left * BLOCKSIZE - 1] & ~static_cast<block_type>(0) << l % BLOCKSIZE), large[p][left]), get_min(large[p][right - (1 << p)], right * BLOCKSIZE + ctz(small[r])));
    }
    else if(left == right) { return get_min((left - 1) * BLOCKSIZE + ctz(small[left * BLOCKSIZE - 1] & ~static_cast<block_type>(0) << l % BLOCKSIZE), left * BLOCKSIZE + ctz(small[r])); }
    else { return right * BLOCKSIZE + ctz(small[r] & ~static_cast<block_type>(0) << l % BLOCKSIZE); }
  }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  vector<int> A(N);
  REP(i,N) cin >> A[i];
  auto f = [&](int a, int b) { return A[a] < A[b]; };
  auto seg = LinearRMQ<decltype(f)>(N,f);
  while (Q--) {
    int l, r; cin >> l >> r;
    cout << A[seg.fold(l, r)] << '\n';
  }

  return 0;
}