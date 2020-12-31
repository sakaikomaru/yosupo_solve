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


template<typename T> struct PersistentStack {
private:
  vector<int> sz{0}, right{-1}, left;
  vector<T> dat;
public:
  int size(int prev) { return sz[prev]; }
  T back(int prev) {
    assert(sz[prev]);
    return dat[right[prev]];
  }
  int push(int prev, T x) {
    dat.push_back(x);
    left.push_back(right[prev]);
    sz.push_back(sz[prev] + 1);
    right.push_back(int(left.size()) - 1);
    return int(sz.size()) - 1;
  }
  int pop(int prev, T x) {
    assert(sz[prev]);
    sz.push_back(sz[prev] - 1);
    right.push_back(left[right[prev]]);
    return int(sz.size()) - 1;
  }
};
template<typename T> struct PersistentQueue {
  vector<T> dat;
  vector<int> d;
  vector<array<int,20>> per;
  int ver = 0;
  int push(T x, int prev) {
    per.push_back(array<int,20>());
    per[ver][0] = prev;
    d.push_back((prev == -1 ? 0 : d[prev]) + 1);
    dat.push_back(x);
    for(int i = 1; i < 20; i++) {
      Int to = per[ver][i - 1];
      if (to == -1) {
        for(int j = i; j < 20; j++) per[ver][j] = -1;
        break;
      }
      per[ver][i] = per[to][i -1 ];
    }
    return ver++;
  }
  T top(int now) {
    assert(now != -1 && d[now]);
    int D = d[now] - 1;
    for (int i = 0; i < 20; i++) if ((D >> i) & 1) now = per[now][i];
    return dat[now];
  }
  T pop(int prev) {
    assert(prev != -1 && d[prev]);
    d.push_back(d[prev] - 1);
    dat.push_back(dat[prev]);
    per.push_back(per[prev]);
    Int now = ver;
    for (int i = 0; i < 20; i++) if ((d[ver] >> i) & 1) now = per[now][i];
    ver++;
    return dat[now];
  }
};


int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  Int Q; cin >> Q;
  Int x, y, z;
  PersistentQueue<Int> perQ;
  LOOP(Q) {
    cin >> x >> y;
    if (x == 0) {
      cin >> z; perQ.push(z, y);
    }
    else {
      z = perQ.top(y); perQ.pop(y);
      cout << z << '\n';
    }
  }
  return 0;
}