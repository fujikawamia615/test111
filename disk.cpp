#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
int FCFS(vector<int> &requests, int head) {
  int totalSeek = 0;
  int currentHead = head;
  cout << "FCFS调度顺序: ";
  for (int i = 0; i < requests.size(); i++) {
    cout << requests[i] << " ";
    totalSeek += abs(requests[i] - currentHead);
    currentHead = requests[i];
  }
  cout << endl;
  return totalSeek;
}
int SSTF(vector<int> &requests, int head) {
  int totalSeek = 0;
  int currentHead = head;
  vector<bool> visited(requests.size(), false);
  vector<int> order;
  for (int i = 0; i < requests.size(); i++) {
    int minDistance = INT_MAX;
    int selectedIndex = -1;
    for (int j = 0; j < requests.size(); j++) {
      if (!visited[j]) {
        int distance = abs(requests[j] - currentHead);
        if (distance < minDistance) {
          minDistance = distance;
          selectedIndex = j;
        }
      }
    }
    visited[selectedIndex] = true;
    order.push_back(requests[selectedIndex]);
    totalSeek += minDistance;
    currentHead = requests[selectedIndex];
  }
  cout << "SSTF调度顺序: ";
  for (int req : order) {
    cout << req << " ";
  }
  cout << endl;
  return totalSeek;
}
int SCAN(vector<int> &requests, int head, int diskSize, bool direction) {
  // true表示向磁道增大的方向移动(向内)，false表示向磁道减小的方向移动(向外)
  int totalSeek = 0;
  int currentHead = head;
  vector<int> left, right;
  vector<int> order;
  for (int req : requests) {
    if (req < head) {
      left.push_back(req);
    } else if (req > head) {
      right.push_back(req);
    }
  }
  sort(left.begin(), left.end());
  sort(right.begin(), right.end());
  if (direction) {
    for (int req : right) {
      order.push_back(req);
      totalSeek += abs(req - currentHead);
      currentHead = req;
    }
    if (!left.empty()) {
      totalSeek += abs(diskSize - 1 - currentHead);
      currentHead = diskSize - 1;
      for (int i = left.size() - 1; i >= 0; i--) {
        order.push_back(left[i]);
        totalSeek += abs(left[i] - currentHead);
        currentHead = left[i];
      }
    }
  } else {
    for (int i = left.size() - 1; i >= 0; i--) {
      order.push_back(left[i]);
      totalSeek += abs(left[i] - currentHead);
      currentHead = left[i];
    }
    if (!right.empty()) {
      totalSeek += abs(currentHead - 0);
      currentHead = 0;
      for (int req : right) {
        order.push_back(req);
        totalSeek += abs(req - currentHead);
        currentHead = req;
      }
    }
  }
  cout << "SCAN调度顺序: ";
  for (int req : order) {
    cout << req << " ";
  }
  cout << endl;
  return totalSeek;
}
int CSCAN(vector<int> &requests, int head, int diskSize) {
  int totalSeek = 0;
  int currentHead = head;
  vector<int> left, right;
  vector<int> order;
  for (int req : requests) {
    if (req < head) {
      left.push_back(req);
    } else if (req > head) {
      right.push_back(req);
    }
  }
  sort(left.begin(), left.end());
  sort(right.begin(), right.end());
  for (int req : right) {
    order.push_back(req);
    totalSeek += abs(req - currentHead);
    currentHead = req;
  }
  if (!left.empty() || !right.empty()) {
    totalSeek += abs(diskSize - 1 - currentHead);
    currentHead = 0;
    totalSeek += (diskSize - 1);
    for (int req : left) {
      order.push_back(req);
      totalSeek += abs(req - currentHead);
      currentHead = req;
    }
  }
  cout << "C-SCAN调度顺序: ";
  for (int req : order) {
    cout << req << " ";
  }
  cout << endl;
  return totalSeek;
}
int main() {
  vector<int> requests;
  int head, diskSize, n;
  cout << "请输入磁盘大小(磁道数): ";
  cin >> diskSize;
  cout << "请输入请求队列长度: ";
  cin >> n;
  cout << "请输入请求队列: ";
  requests.resize(n);
  for (int i = 0; i < n; i++) {
    cin >> requests[i];
  }
  cout << "请输入磁头初始位置: ";
  cin >> head;
  cout << "\n磁盘调度算法演示\n";
  cout << "========================\n";
  cout << "磁盘大小: " << diskSize << endl;
  cout << "请求队列: ";
  for (int req : requests) {
    cout << req << " ";
  }
  cout << "\n磁头初始位置: " << head << "\n" << endl;
  int fcfsSeek = FCFS(requests, head);
  cout << "FCFS算法总寻道距离: " << fcfsSeek << endl;
  cout << "FCFS算法平均寻道距离: " << (double)fcfsSeek / requests.size() << "\n"
       << endl;
  int sstfSeek = SSTF(requests, head);
  cout << "SSTF算法总寻道距离: " << sstfSeek << endl;
  cout << "SSTF算法平均寻道距离: " << (double)sstfSeek / requests.size() << "\n"
       << endl;
  int scanSeek = SCAN(requests, head, diskSize, true);
  cout << "SCAN算法总寻道距离: " << scanSeek << endl;
  cout << "SCAN算法平均寻道距离: " << (double)scanSeek / requests.size() << "\n"
       << endl;
  int cscanSeek = CSCAN(requests, head, diskSize);
  cout << "C-SCAN算法总寻道距离: " << cscanSeek << endl;
  cout << "C-SCAN算法平均寻道距离: " << (double)cscanSeek / requests.size()
       << "\n"
       << endl;
  return 0;
}