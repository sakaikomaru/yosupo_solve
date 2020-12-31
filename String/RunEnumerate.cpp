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


inline vector<int> z_algorithm(const string& s) {
  int n = (int)s.length();
  vector<int> z(n);
  z[0] = n;
  for (int i = 1, l = 0, r = 0; i < n; i++) {
    if (i <= r) z[i] = std::min(r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return z;
}
void enum_run_rec(int l, int r, string& s, vector<tuple<int,int,int>>& runs) {
  if (r - l <= 1) return;
  int m = (l + r) / 2;
  enum_run_rec(l,m,s,runs);
  enum_run_rec(m,r,s,runs);
  auto f = [&](bool rev) {
    string t = s.substr(l, r - l);
    if (rev) {
      reverse(t.begin(),t.end());
      m = l + r - m;
    }
    int len = r - l, mid = m - l;
    string tl = t.substr(0, mid);
    reverse(tl.begin(), tl.end());
    string tr = t.substr(mid, len - mid) + t;
    auto zl = z_algorithm(tl);
    auto zr = z_algorithm(tr);
    zl.push_back(0);
    for (int k = 1; mid - k >= 0; k++) {
      int li = m - k - zl[k];
      int ri = m + min((r - m), zr[len - k]);
      if (rev) {
        swap(li, ri);
        li = l + r - li;
        ri = l + r - ri;
      }
      if (ri - li < 2 * k) continue;
      if (li > 0 && s[li - 1] == s[li - 1 + k]) continue;
      if (ri < (int)s.size() && s[ri] == s[ri - k]) continue;
      runs.push_back(make_tuple(li, ri, k));
    }
  };
  f(false);
  f(true);
}
vector<tuple<int,int,int>> enum_run(string& s) {
  vector<tuple<int,int,int>> runs;
  enum_run_rec(0, s.size(), s, runs);
  sort(runs.begin(), runs.end());
  vector<tuple<int,int,int>> ret;
  for (int i = 0; i < (int)runs.size(); i++) {
    if (i > 0 && get<0>(runs[i]) == get<0>(runs[i - 1]) && get<1>(runs[i]) == get<1>(runs[i - 1])) continue;
    int l = get<0>(runs[i]);
    int r = get<1>(runs[i]);
    int t = get<2>(runs[i]);
    ret.push_back(make_tuple(t, l, r));
  }
  return ret;
}


int main() {
  string s; cin >> s;
  auto runs = enum_run(s);
  sort(ALL(runs));
  int N = (int)runs.size();
  cout << N << '\n';
  REP(i,N) {
    int t = get<0>(runs[i]);
    int l = get<1>(runs[i]);
    int r = get<2>(runs[i]);
    cout << t << ' ' << l << ' ' << r << '\n';
  }
  return 0;
}