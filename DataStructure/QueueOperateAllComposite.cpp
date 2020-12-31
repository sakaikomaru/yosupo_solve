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


template <class SemiGroup>
class SlidingWindowAggregation {
public:
  using structure       = SemiGroup;
  using value_semigroup = typename SemiGroup::value_structure;
  using value_type      = typename SemiGroup::value_structure::type;
  using size_type       = size_t;

private:
  template <class T, class U = void> struct has_identity : public std::false_type {};
  template <class T> struct has_identity<T, typename std::conditional<false, decltype(T::identity()), void>::type> : public std::true_type {};
  template <class T, typename std::enable_if<has_identity<T>::value, void>::type* = nullptr> static typename T::type S_empty_exception() { return T::identity(); }
  template <class T, typename std::enable_if<!has_identity<T>::value, void>::type* = nullptr> static typename T::type S_empty_exception() { throw std::runtime_error("attempted to fold empty queue"); }

  class node_type {
  public:
    value_type value, sum;
    node_type(const value_type& value, const value_type& sum) : value(value), sum(sum) { }
  };

  std::stack<node_type> M_front, M_back;

public:
  SlidingWindowAggregation(): M_front(), M_back() { }
  value_type fold() const {
    if (empty()) return S_empty_exception<value_semigroup>();
    if (M_front.empty()) return M_back.top().sum;
    else if (M_back.empty()) return M_front.top().sum;
    return value_semigroup::operation(M_front.top().sum, M_back.top().sum);
  }
  void push(const value_type& x) {
    if (M_back.empty()) M_back.emplace(x, x);
    else {
      value_type tmp = value_semigroup::operation(M_back.top().sum, x);
      M_back.emplace(x, tmp);
    }
  }
  void pop() {
    if (M_front.empty()) {
      M_front.emplace(M_back.top().value, M_back.top().value);
      M_back.pop();
      while (!M_back.empty()) {
        value_type tmp = value_semigroup::operation(M_back.top().value, M_front.top().sum);
        M_front.emplace(M_back.top().value, tmp);
        M_back.pop();
      }
    }
    M_front.pop();
  }
  size_type size() const { return M_front.size() + M_back.size(); }
  bool empty() const { return M_front.empty() && M_back.empty(); }
};


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

struct st_monoid {
  struct value_structure {
    using type = std::pair<mint, mint>;
    static type identity() { return { mint(1), mint(0) }; }
    static type operation(const type& v1, const type& v2) {
      return { v2.first * v1.first, v2.first * v1.second + v2.second };
    }
  };
};

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int Q; cin >> Q;
  SlidingWindowAggregation<st_monoid> que;

  LOOP(Q) {
    int t; cin >> t;
    if (t == 0) {
      mint a, b;
      cin >> a >> b; que.push({ a, b });
    }
    else if (t == 1) { que.pop(); }
    else {
      mint x; cin >> x;
      auto y = que.fold();
      cout << (y.first * x + y.second) << '\n';
    }
  }

  return 0;
}