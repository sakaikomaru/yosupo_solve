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


vector<int> manacher_odd(const string& s) {
  int n = (int)s.size();
  vector<int> d(n);
  for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = i > r ? 1 : min(d[l + r - i], r - i + 1);
    while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
    d[i] = k--;
    if (i + k > r) {
      r = i + k;
      l = i - k;
    }
  }
  return d;
}
vector<int> manacher_even(const string& s) {
  int n = (int)s.size();
  vector<int> d(n);
  for (int i = 0, l = 0, r = -1; i < n; i++) {
    int k = i > r ? 0 : min(d[l + r - i + 1], r - i + 1);
    while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
    d[i] = k--;
    if (i + k > r) {
      r = i + k;
      l = i - k - 1;
    }
  }
  return d;
}
vector<int> palindrome(const string& s) {
  int n = (int)s.size();
  if (n == 1) return {1};
  vector<int> d1 = manacher_odd(s);
  vector<int> d2 = manacher_even(s);
  vector<int> d(2 * n - 1);
  for (int i = 0; i < 2 * n - 1; i++) {
    if (i & 1) d[i] = d2[i / 2 + 1] * 2;
    else d[i] = d1[i / 2] * 2 - 1;
  }
  return d;
}
pair<vector<int>,vector<int>> palindromes(const string& s) {
  int n = s.size();
  vector<int> xs((n + 1), 0);
  vector<int> ys(n, 0);
  for (int z = 0; z < 2; z++) {
    int l = 0, r = 0;
    for (int i = 0; i < n; i++) {
      int zz = (z == 1) ? 0 : 1;
      int t = r - i + zz;
      if (i < r) {
        if (z == 0) xs[i] = min(t, xs[l + t]);
        else        ys[i] = min(t, ys[l + t]);
      }
      int L, R;
      if (z == 0) {
        L = i - xs[i];
        R = i + xs[i] - zz;
      }
      else {
        L = i - ys[i];
        R = i + ys[i] - zz;
      }
      while (L != 0 && R + 1 < n && s[L-1] == s[R+1]) {
        if (z == 0) xs[i]++;
        else        ys[i]++;
        L--;
        R++;
      }
      if (R > r) l = L, r = R;
    }
  }
  auto p = make_pair(xs, ys);
  return p;
}
vector<int> enumerate_parindromes(const string& s) {
  auto ps = palindromes(s);
  auto even = ps.first;
  auto odd = ps.second;
  vector<int> res(2 * s.size() - 1, 0);
  even.erase(even.begin());
  for (int i = 0; i < 2 * s.size() - 1; i++) {
    if (i & 1) res[i] = 2 * even[i / 2];
    else       res[i] = 2 * odd[i / 2] + 1;
  }
  return res;
}

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  string s; cin >> s;
  auto pa = palindrome(s);
  for (const auto& p : pa) cout << p << ' ';
  cout << '\n';

  return 0;
}