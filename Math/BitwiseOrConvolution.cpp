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

template <std::uint_fast64_t Modulus>
class modint {
  using u32 = std::uint_fast32_t;
  using u64 = std::uint_fast64_t;
  using i64 = std::int_fast64_t;
  inline u64 apply(i64 x) { return (x < 0 ? x + Modulus : x); };
public:
  u64 a;
  static constexpr u64 mod = Modulus;
  constexpr modint(const i64& x = 0) noexcept: a(apply(x % (i64)Modulus)) {}
  constexpr modint operator+(const modint& rhs) const noexcept { return modint(*this) += rhs; }
  constexpr modint operator-(const modint& rhs) const noexcept { return modint(*this) -= rhs; }	
  constexpr modint operator*(const modint& rhs) const noexcept { return modint(*this) *= rhs; }
  constexpr modint operator/(const modint& rhs) const noexcept { return modint(*this) /= rhs; }
  constexpr modint operator^(const u64& k) const noexcept { return modint(*this) ^= k; }
  constexpr modint operator^(const modint& k) const noexcept { return modint(*this) ^= k.value(); }
  constexpr modint operator-() const noexcept { return modint(Modulus - a); }
  constexpr modint operator++() noexcept { return (*this) = modint(*this) + 1; }
  constexpr modint operator--() noexcept { return (*this) = modint(*this) - 1; }
  const bool operator==(const modint& rhs) const noexcept { return a == rhs.a; };
  const bool operator!=(const modint& rhs) const noexcept { return a != rhs.a; };
  const bool operator<=(const modint& rhs) const noexcept { return a <= rhs.a; };
  const bool operator>=(const modint& rhs) const noexcept { return a >= rhs.a; };
  const bool operator<(const modint& rhs) const noexcept { return a < rhs.a; };
  const bool operator>(const modint& rhs) const noexcept { return a > rhs.a; };
  constexpr modint& operator+=(const modint& rhs) noexcept {
    a += rhs.a;
    if (a >= Modulus) a -= Modulus;
    return *this;
  }
  constexpr modint& operator-=(const modint& rhs) noexcept {
    if (a < rhs.a) a += Modulus;
    a -= rhs.a;
    return *this;
  }
  constexpr modint& operator*=(const modint& rhs) noexcept {
    a = a * rhs.a % Modulus;
    return *this;
  }
  constexpr modint& operator/=(modint rhs) noexcept {
    u64 exp = Modulus - 2;
    while (exp) {
      if (exp % 2) (*this) *= rhs;
      rhs *= rhs;
      exp /= 2;
    }
    return *this;
  }
  constexpr modint& operator^=(u64 k) noexcept {
    auto b = modint(1);
    while(k) {
      if(k & 1) b = b * (*this);
      (*this) *= (*this);
      k >>= 1;
    }
    return (*this) = b;
  }
  constexpr modint& operator=(const modint& rhs) noexcept {
    a = rhs.a;
    return (*this);
  }
  constexpr u64& value() noexcept { return a; }
  constexpr const u64& value() const noexcept { return a; }
  explicit operator bool() const { return a; }
  explicit operator u32() const { return a; }
  const modint inverse() const { return modint(1) / *this; }
  const modint pow(i64 k) const { return modint(*this) ^ k; }
  friend std::ostream& operator<<(std::ostream& os, const modint& p) { return os << p.a; }
  friend std::istream& operator>>(std::istream& is, modint& p) {
    u64 t;
    is >> t;
    p = modint(t);
    return is;
  }
};

using mint = modint<998244353>;


template<typename T>
void fztOR(vector<T>& a, int m) { for (int d = 1; d < m; d <<= 1) REP(mask,m) if ((mask & d) == 0) a[mask | d] += a[mask]; }
template<typename T>
void fmtOR(vector<T>& a, int m) { for (int d = 1; d < m; d <<= 1) REP(mask,m) if ((mask & d) == 0) a[mask | d] -= a[mask]; }
template<typename T>
void OR_convolution(vector<T>& a, vector<T>& b, int m) {
  fztOR(a,m);
  fztOR(b,m);
  REP(i,m) a[i] *= b[i];
  fmtOR(a,m);
}


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int n; cin >> n;
  int m = 1 << n;
  vector<mint> a(m), b(m);
  REP(i,m) cin >> a[i];
  REP(i,m) cin >> b[i];
  OR_convolution(a, b, m);
  REP(i,m) cout << a[i] << ' ';
  cout << '\n';
  
  return 0;
}