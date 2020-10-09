#include "bits/stdc++.h"

using namespace std;

using T = unsigned long long;

class Node {
 public:
  T state;
  T parent;
  char action;
  int cost;
  int h;
  Node(T a, T b, char c, int d, int e) {
    state = a;
    parent = b;
    action = c;
    cost = d;
    h = e;
  }
  bool operator<(const Node& rhs) const { return cost + h > rhs.cost + rhs.h; }  
};

const char* dir = "LRUD";
const int dr[] = { 0,  0, -1, +1};
const int dc[] = {-1, +1,  0,  0};

const int LENGTH = 4;
const int SIZE = LENGTH * LENGTH;
const int BITS = 4;
const int ONES = (1 << BITS) - 1;
const T GOAL = 17134975606245761295;

bool solvable(vector<int>& p) {
  int inv = 0;
  for (int i = 0; i < SIZE; ++i) {
    for (int j = i + 1; j < SIZE; ++j) {
      if (p[i] && p[j] && p[i] > p[j])
        ++inv;
    }
  }

  if (LENGTH % 2 == 1) {
    return inv % 2 == 0;
  } else {
    int row = LENGTH - ((find(p.begin(), p.end(), 0) - p.begin()) / LENGTH);
    return row % 2 != inv % 2;
  }
}

inline bool valid(int x, int y) { 
  return x >= 0 && x < LENGTH && y >= 0 && y < LENGTH;
}

T myHash(vector<int>& a) {
  T mask = 0;
  for (int i = 0; i < SIZE; ++i) {
    mask |= (T)i << (a[i] * BITS);
  }
  return mask;
}

int manhattanDistance(T a, T b) {
  int ret = 0;
  a >>= BITS;
  b >>= BITS;
  for (int i = 0; i < SIZE - 1; ++i, a >>= BITS, b >>= BITS) {
    int pos1 = a & ONES;
    int pos2 = b & ONES;
    ret += abs(pos1 / LENGTH - pos2 / LENGTH);
    ret += abs(pos1 % LENGTH - pos2 % LENGTH);
  }
  return ret;
}

vector<pair<T, int>> actions(T state) {
  vector<pair<T, int>> ret;

  int pos = state & ONES; // Position of empty tile
  int row = pos / LENGTH;
  int col = pos % LENGTH;

  for (int direction = 0; direction < 4; ++direction) {
    int new_row = row + dr[direction];
    int new_col = col + dc[direction];

    if (valid(new_row, new_col)) {
      int new_pos = new_row * LENGTH + new_col;
      int swap_pos = -1;

      for (int i = 0; i < SIZE; ++i) {
        if ((int)((state >> (i * BITS)) & ONES) == new_pos) {
          swap_pos = i;
          break;
        }
      }

      assert(swap_pos != -1);

      T new_state = state;
      new_state &= ~ONES;
      new_state &= ~((T)ONES << (swap_pos * BITS));
      new_state |= new_pos;
      new_state |= (T)pos << (swap_pos * BITS);

      ret.emplace_back(new_state, direction);
    }
  }

  return ret;
}

string solve(T source) {
  priority_queue<Node> q[2];
  map<T, pair<T, char>> last[2]; // Stores parent and action
  
  T goal[2] = {GOAL, source};

  // $ for no action
  q[0].emplace(source, 0, '$', 0, manhattanDistance(source, GOAL));
  q[1].emplace(GOAL  , 0, '$', 0, manhattanDistance(source, GOAL));

  vector<T> candidates;
  bool done = false;

  while (!done) {
    done = true;

    for (int i = 0; i < 2; ++i) {
      if (!q[i].empty()) {
        done = false;

        auto top = q[i].top();
        q[i].pop();

        if (last[i].count(top.state))
          continue;
        
        last[i][top.state] = {top.parent, top.action};
        
        candidates.push_back(top.state);

        for (auto &action : actions(top.state)) {
          if (!last[i].count(action.first)) {
            q[i].emplace(
              action.first,
              top.state,
              action.second,
              top.cost + 1,
              manhattanDistance(action.first, goal[i])
            );
          }
        }
      }
    }

    for (auto &candidate : candidates) {
      if (last[0].count(candidate) && last[1].count(candidate)) {
        string ret;

        T cur = candidate;
        while (true) {
          auto prev = last[0][cur];
          if (prev.second == '$')
            break;
          
          ret.push_back(dir[(int)prev.second]);
          cur = prev.first;
        }

        reverse(ret.begin(), ret.end());

        cur = candidate;
        while (true) {
          auto prev = last[1][cur];
          if (prev.second == '$')
            break;
          
          ret.push_back(dir[prev.second ^ 1]);
          cur = prev.first;
        }

        return ret;
      }
    }

    candidates.clear();
  }

  assert(false);
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while (t--) {
    vector<int> puzzle(SIZE);
    for (auto &x : puzzle) {
      cin >> x;
    }
    if (solvable(puzzle)) {
      cout << solve(myHash(puzzle)) << '\n';
    } else {
      cout << "This puzzle is not solvable.\n";
    }
  }

  return 0;
}
