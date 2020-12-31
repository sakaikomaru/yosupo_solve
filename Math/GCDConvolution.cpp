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

using mint = modint<1'000'000'007>;


/* https://www.codechef.com/problems/GCDSUM */

template <typename T>
void fztDIV(vector<T>& f, int m) {
  VB sieve(m, true);
  for (int i = 2; i < m; i++) {
    if (sieve[i]) {
      for (int j = (m - 1) / i; j > 0; j--) {
        sieve[j * i] = false;
        f[j] += f[j * i];
      }
    }
  }
}
template <typename T>
auto fmtDIV(vector<T>& f, int m) {
  VB sieve(m, true);
  for (int i = 2; i < m; i++) {
    if (sieve[i]) {
      for (int j = 1; j * i < m; j++) {
        sieve[j * i] = false;
        f[j] -= f[i * j];
      }
    }
  }
}
template <typename T>
vector<T> GCD_convolution(vector<T>& f, vector<T>& g, int m) {
  vector<T> ret(m);
  for(int i = 1; i < m; i++) ret[i] += f[i] + g[i];

  f = fztDIV(f);
  g = fztDIV(g);
  for (int i = 0; i < m; i++) f[i] *= g[i];
  f = fmtDIV(f);
  for (int i = 0; i < m; i++) ret[i] += f[i];
  return ret;
}

int main(){
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, M; cin >> N >> M;

  auto a = vector<vector<int>>(N, vector<int>(M));
  REP(i,N) REP(j,M) cin >> a[i][j];

  vector<mint> res(100001);
  res[0] = 1;

  REP(i,N) {
    vector<mint> temp(100001);
    temp[0] = 1;
    for (int x : a[i]) temp[x] += 1;
    res = GCD_convolution(res, temp, 100001);
  }

  mint ans = 0;

  for (int i = 1; i <= 100000; ++i) ans += res[i] * i;
  for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) ans -= a[i][j];

  cout << ans << "\n";

  return 0;
}

/**
* 数列がn個ある。各数列の要素数は全てm個である。
* このとき異なる数列から2つずつ全ての組み合わせについて最大公約数を取り、
* その和を mod 1000000007 で答えよ。
* 制約:
* 1 <= n <= 20
* 1 <= m <= 100000
* 1 <= {aij} <= 100000
*/