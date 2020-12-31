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


namespace Monoid {


template<class T, T id = T{}> struct add_monoid {
  using value_type = T;
  T a;
  constexpr add_monoid(T a): a(a) {}
  static constexpr add_monoid operation(const add_monoid& l, const add_monoid& r) { return add_monoid{ l.a + r.a }; }
  static constexpr add_monoid identity() { return add_monoid{id}; };
  static constexpr add_monoid inverse(const add_monoid& x) { return add_monoid{-x.a}; }
};
template<class T, T id = T{1}> struct mul_monoid {
  using value_type = T;
  T a;
  constexpr mul_monoid(T a): a(a) {}
  static constexpr mul_monoid operation(const mul_monoid& l, const mul_monoid& r) { return mul_monoid{ l.a * r.a }; }
  static constexpr mul_monoid identity() { return mul_monoid{id}; };
};
template<class T, T id = T{}> struct max_monoid {
  using value_type = T;
  T a;
  constexpr max_monoid(T a): a(a) {}
  static constexpr max_monoid operation(const max_monoid& l, const max_monoid& r) { return max_monoid{ std::max(l.a, r.a) }; }
  static constexpr max_monoid identity() { return max_monoid{id}; };
};
template<class T, T id = T{}> struct min_monoid {
  using value_type = T;
  T a;
  constexpr min_monoid(T a): a(a) {}
  static constexpr min_monoid operation(const min_monoid& l, const min_monoid& r) { return min_monoid{ std::min(l.a, r.a) }; }
  static constexpr min_monoid identity() { return min_monoid{id}; };
};
template<class T, T& id> struct monoid {
  using value_type = T;
  T a;
  constexpr monoid(T a): a(a) {}
  static constexpr monoid operation(const monoid& l, const monoid& r) { return monoid{ l.a + r.a }; }
  static constexpr monoid identity() { return monoid{id}; }
  static constexpr monoid inverse(const monoid& x) { return monoid{ x.a.inverse() }; }
};
template<class A, class B> struct cartesian_product_monoid {
  using value_type = std::pair<typename A::value_type, typename B::value_type>;
  value_type a;
  constexpr cartesian_product_monoid(const value_type& a): a(a) {}
  static constexpr cartesian_product_monoid operation(const cartesian_product_monoid& l, const cartesian_product_monoid& r) {
    return cartesian_product_monoid{ { A::operation(l.a.first, r.a.first).a, B::operation(l.a.second, r.a.second).a } };
  }
  static constexpr cartesian_product_monoid identity() { return cartesian_product_monoid{ { A::identity().a, B::identity().a } }; }
  static constexpr cartesian_product_monoid inverse(const cartesian_product_monoid& x) {
    return cartesian_product_monoid{ { A::inverse(x.a.first).a, B::inverse(x.a.second).a } };
  }
};
template<class T> struct affine {
  using value_type = T;
  value_type a;
  value_type b;
  constexpr affine(const value_type& a = 1, const value_type& b = 0) : a(a), b(b) {}
  constexpr affine operator+(const affine& r) const { return affine{ a + r.a, b + r.b }; }
  constexpr affine composite(const affine& r) const { return affine{ a * r.a, a * r.b + b }; }
  constexpr value_type evaluate(const value_type& x) { return a * x + b; }
};
template<class T> struct affine_composite_monoid {
  using value_type = affine<T>;
  value_type a;
  constexpr affine_composite_monoid(const value_type& a): a(a) {}
  static constexpr affine_composite_monoid operation(const affine_composite_monoid& l, const affine_composite_monoid& r) {
    return affine_composite_monoid{ r.a.composite(l.a) };
  }
  static constexpr affine_composite_monoid identity() {
    return affine_composite_monoid{value_type()};
  }
};


} // namespace Monoid


template<typename Monoid>
class SegmentTree {
public:
  using value_type = Monoid;
  using T = typename value_type::value_type;
  using usize = uint_fast32_t;
private:
  int n;
  vector<value_type> data;
private:
  usize base() const { return data.size() >> 1; }
public:
  SegmentTree() = default;
  explicit SegmentTree(usize n_) : n(n_) {
    usize size = 1;
    while (size <= n) size <<= 1;
    data.assign(size << 1, value_type::identity());
  }
  template<class InputIt>
  explicit SegmentTree(InputIt first, InputIt last) : SegmentTree(std::distance(first, last)) {
    for(int index = 0; first != last; first++, index++) set(index, *first);
    build();
  }
  usize size() const { return n; }
  bool empty() const { return size() == 0; }
  void clear() {
    n = 0;
    data.clear();
  }
  void swap(SegmentTree& r) {
    std::swap(n, r.n);
    data.swap(r.data);
  }
  T get(usize i) const { return data[i + base()].a; }
  void set(usize i, const value_type& x) { data[i + base()] = x; }
  void build() { for(usize i = (int)base() - 1; i > 0; i--) data[i] = value_type::operation(data[i * 2 + 0], data[i * 2 + 1]); }
  void change(usize i, const value_type& x) {
    data[i += base()] = x;
    while(i >>= 1) data[i] = value_type::operation(data[i * 2 + 0], data[i * 2 + 1]);
  }
  void update(usize i, const value_type& x) { change(i, value_type::operation(get(i), x)); }
  T fold(usize first, usize last) const {
    first += base();
    last += base();
    value_type lval = value_type::identity();
    value_type rval = value_type::identity();
    while(first != last) {
      if(first & 1) lval = value_type::operation(lval, data[first++]);
      if(last  & 1) rval = value_type::operation(data[--last], rval);
      first >>= 1;
      last  >>= 1;
    }
    return value_type::operation(lval, rval).a;
  }
  T fold_all() const { return data[1].a; }
  template<class F>
  usize search_right(int l, const F& f) const {
    if(l == size()) return base();
    l += base();
    value_type acc = value_type::identity();
    do {
      while(l % 2 == 0) l >>= 1;
      if(!f(value_type::operation(acc, data[l]).a)) {
        while(l < base()) {
          l = l << 1;
          if(f(value_type::operation(acc, data[l]).a)) {
            acc = value_type::operation(acc, data[l]);
            l += 1;
          }
        }
        return l - base();
      }
      acc = value_type::operation(acc, data[l]);
      l += 1;
    } while((l & -l) != l);
    return size();
  }
  template<class F>
  usize search_left(int r, const F& f) const {
    if(r == 0) return 0;
    r += base();
    value_type acc = value_type::identity();
    do {
      r--;
      while(r > 1 and (r % 2)) r >>= 1;
      if(!f(value_type::operation(data[r], acc).a)) {
        while(r < base()) {
          r = r * 2 + 1;
          if(f(value_type::operation(data[r], acc).a)) {
            acc = value_type::operation(data[r], acc);
            r -= 1;
          }
        }
        return r + 1 - base();
      }
      acc = value_type::operation(data[r], acc);
    } while((r & -r) == r);
    return 0;
  }
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

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  SegmentTree<Monoid::affine_composite_monoid<mint>> seg(N);

  REP(i,N) {
    int a, b; cin >> a >> b;
    seg.set(i, Monoid::affine<mint>(a, b));
  }
  seg.build();
  while (Q--) {
    int t, x, y, z; cin >> t >> x >> y >> z;
    if (t == 0) seg.change(x, Monoid::affine<mint>(y, z));
    if (t == 1) cout << seg.fold(x, y).evaluate(z).value() << '\n';
  }
  return 0;
}