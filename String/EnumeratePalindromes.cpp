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


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  string s; cin >> s;
  auto pa = palindrome(s);
  for (const auto& p : pa) cout << p << ' ';
  cout << '\n';

  return 0;
}