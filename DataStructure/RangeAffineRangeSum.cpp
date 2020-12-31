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


template<class M, class E, class operation>
class LazySegmentTree {
public:
  using value_type    = M;
  using T             = typename value_type::value_type;
  using operator_type = E;
  using usize         = std::uint_fast32_t;
  struct NodeType {
    value_type value;
    operator_type lazy;
    NodeType(const value_type& value, const operator_type& lazy): value(value), lazy(lazy) {}
  };
private:
  vector<NodeType> data;
  value_type value(const NodeType& x) const { return value_type{ operation()(x.value.a, x.lazy.a) }; }
  usize countr_zeros(usize x) const { return __builtin_ctzll(x); }
  void add(operator_type& l, const operator_type& r) { l = operator_type::operation(l, r); }
  void propagate(usize idx) {
    add(data[idx * 2 + 0].lazy, data[idx].lazy);
    add(data[idx * 2 + 1].lazy, data[idx].lazy);
    data[idx].lazy = operator_type::identity();
  }
  void propagate_bound(usize idx) {
    if(idx == 0) return;
    std::stack<usize> order;
    idx >>= countr_zeros(idx);
    while (idx >>= 1) order.push(idx);
    while (!order.empty()) {
      propagate(order.top());
      order.pop();
    }
  }
  void recalc(usize idx) { data[idx].value = value_type::operation(value(data[idx * 2 + 0]), value(data[idx * 2 + 1])); }
  void recalc_bound(usize idx) {
    if(idx == 0) return;
    idx >>= countr_zeros(idx);
    while(idx >>= 1) recalc(idx);
  }
public:
  LazySegmentTree() = default;
  explicit LazySegmentTree(usize n) : data(n << 1, NodeType(value_type::identity(), operator_type::identity())) {}
  template<class InputIt>
  explicit LazySegmentTree(InputIt first, InputIt last) : LazySegmentTree(std::distance(first, last)) {
    for(int index = 0; first != last; first++, index++) set(index, *first);
    build();
  }
  usize size() { return data.size() >> 1; }
  bool empty() { return size() == 0; }
  void clear() { data.clear(); }
  void swap(LazySegmentTree& r) { data.swap(r.data); }
  T get(usize i) { return fold(i, i + 1).value.a; }
  void set(usize i, const value_type& x) { data[i + size()].value = x; };
  void build() { for(usize i = size() - 1; i > 0; i--) recalc(i); }
  void change(usize i, const value_type& x) {
    i += size();
    propagate_bound(((i >> 1) << 1) + 1);
    data[i] = NodeType(x, operator_type::identity());
    recalc_bound(((i >> 1) << 1) + 1);
  }
  void update(usize i, const value_type& x) {
    i += size();
    propagate_bound(((i >> 1) << 1) + 1);
    data[i] = NodeType(value_type::operation(value(data[i]), x), operator_type::identity());
    recalc_bound(((i >> 1) << 1) + 1);
  }
  T fold(usize first, usize last) {
    first += size();
    last  += size();
    propagate_bound(first);
    recalc_bound(first);
    propagate_bound(last);
    recalc_bound(last);
    value_type lval = value_type::identity();
    value_type rval = value_type::identity();
    while(first != last) {
      if(first & 1) lval = value_type::operation(lval, value(data[first++]));
      if(last  & 1) rval = value_type::operation(value(data[--last]), rval);
      first >>= 1;
      last  >>= 1;
    }
    return value_type::operation(lval, rval).a;
  }
  void update(usize first, usize last, const operator_type& x) {
    first += size();
    last  += size();
    usize first_tmp = first;
    usize last_tmp  = last;
    propagate_bound(first);
    propagate_bound(last);
    while(first != last) {
      if(first & 1) add(data[first++].lazy, x);
      if(last  & 1) add(data[--last].lazy, x);
      first >>= 1;
      last >>= 1;
    }
    recalc_bound(first_tmp);
    recalc_bound(last_tmp);
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
mint id = 0;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  int N, Q; cin >> N >> Q;
  using A = Monoid::cartesian_product_monoid<Monoid::monoid<mint, id>, Monoid::monoid<mint,id>>;
  using B = Monoid::affine_composite_monoid<mint>;
  struct operation {
    std::pair<mint, mint> operator()(std::pair<mint, mint> a, Monoid::affine<mint> b) {
      return { a.first * b.a + a.second * b.b, a.second };
    };
  };

  LazySegmentTree<A,B,operation> seg(N);

  REP(i,N) {
    int a; cin >> a;
    seg.set(i, A({a,1}));
  }
  seg.build();

while (Q--) {
  int t; cin >> t;
  if (t) {
    int l, r; cin >> l >> r;
    cout << seg.fold(l, r).first.value() << '\n';
  }
  else {
    int l, r, b, c; cin >> l >> r >> b >> c;
    seg.update(l, r, Monoid::affine<mint>(b, c));
  }
}

  return 0;
}