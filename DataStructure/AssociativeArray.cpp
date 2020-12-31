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

template<typename K, typename V>
class HashMap {
public:
  HashMap() = default;
  V& operator[](const K& k) {
    for (Word32 i = hash(k);; (i += 1) &= (N - 1)) {
      if (not m_used.test(i)) {
        m_keys[i] = k, m_used.set(i);
        return m_vals[i] = V{};
      }
      if (m_keys[i] == k) { return m_vals[i]; }
    }
  }
private:
  static constexpr int LG = 20;
  static constexpr int N = 1 << LG;
  static constexpr Word64 r = 11995408973635179863ULL;
  static constexpr Word32 hash(const Word64 a) { return (a * r) >> (64 - LG); }
  bitset<N> m_used;
  K m_keys[N];
  V m_vals[N];
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  HashMap<Word,Word> dict;
  Int Q; cin >> Q;
  while (Q--) {
    Int q; cin >> q;
    if (q == 0) {
      Word k, v; cin >> k >> v;
      dict[k] = v;
    }
    else {
      Word k; cin >> k;
      cout << dict[k] << '\n';
    }
  }
  return 0;
}