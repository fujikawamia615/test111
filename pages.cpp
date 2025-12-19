#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;
int opt(vector<int> &pages, int frames) {
  cout << "OPT算法执行过程:" << endl;
  int n = pages.size();
  vector<int> memory(frames, -1);
  int faults = 0;
  int occupiedCount = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    bool found = false;
    for (int j = 0; j < occupiedCount; j++) {
      if (memory[j] == page) {
        found = true;
        break;
      }
    }
    cout << "访问页面 " << page << ": ";
    if (!found) {
      faults++;
      if (occupiedCount < frames) {
        memory[occupiedCount] = page;
        occupiedCount++;
        cout << "装入页面 " << page << " 到位置 " << occupiedCount - 1;
      } else {
        int farthest = i;
        int replaceIndex = 0;
        for (int j = 0; j < frames; j++) {
          int nextUse = INT_MAX;
          for (int k = i + 1; k < n; k++) {
            if (memory[j] == pages[k]) {
              nextUse = k;
              break;
            }
          }
          if (nextUse > farthest) {
            farthest = nextUse;
            replaceIndex = j;
          }
        }
        cout << "替换页面 " << memory[replaceIndex] << " 为页面 " << page
             << " (位置 " << replaceIndex << ")";
        memory[replaceIndex] = page;
      }
      cout << " [缺页]";
    } else {
      cout << "页面已在内存中";
    }
    cout << " 内存状态: [";
    for (int j = 0; j < frames; j++) {
      if (j > 0)
        cout << " ";
      if (memory[j] == -1)
        cout << "-";
      else
        cout << memory[j];
    }
    cout << "]" << endl;
  }
  return faults;
}
int fifo(vector<int> &pages, int frames) {
  cout << "FIFO算法执行过程:" << endl;
  int n = pages.size();
  list<int> frame;
  unordered_map<int, bool> inFrame;
  int faults = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    cout << "访问页面 " << page << ": ";
    if (!inFrame[page]) {
      faults++;
      if (frame.size() == frames) {
        int removed = frame.front();
        frame.pop_front();
        inFrame[removed] = false;
        cout << "移除页面 " << removed << ", ";
      }
      frame.push_back(page);
      inFrame[page] = true;
      cout << "装入页面 " << page;
      cout << " [缺页]";
    } else {
      cout << "页面已在内存中";
    }
    cout << " 内存状态: [";
    int count = 0;
    for (int f : frame) {
      if (count > 0)
        cout << " ";
      cout << f;
      count++;
    }
    // 填充空位
    while (count < frames) {
      if (count > 0)
        cout << " ";
      cout << "-";
      count++;
    }
    cout << "]" << endl;
  }
  return faults;
}
int lru(vector<int> &pages, int frames) {
  cout << "LRU算法执行过程:" << endl;
  int n = pages.size();
  list<int> frame;
  unordered_map<int, list<int>::iterator> pagePos;
  int faults = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    cout << "访问页面 " << page << ": ";
    if (pagePos.find(page) == pagePos.end()) {
      faults++;
      if (frame.size() == frames) {
        int removed = frame.front();
        frame.pop_front();
        pagePos.erase(removed);
        cout << "移除页面 " << removed << ", ";
      }
      cout << "装入页面 " << page;
      cout << " [缺页]";
    } else {
      frame.erase(pagePos[page]);
      cout << "页面 " << page << " 移至最近使用位置";
    }
    frame.push_back(page);
    pagePos[page] = --frame.end();
    cout << " 内存状态: [";
    int count = 0;
    for (int f : frame) {
      if (count > 0)
        cout << " ";
      cout << f;
      count++;
    }
    while (count < frames) {
      if (count > 0)
        cout << " ";
      cout << "-";
      count++;
    }
    cout << "]" << endl;
  }
  return faults;
}
int lfu(vector<int> &pages, int frames) {
  cout << "LFU算法执行过程:" << endl;
  int n = pages.size();
  vector<int> memory(frames, -1);
  vector<int> frequency(frames, 0);
  vector<int> timeStamp(frames, 0);
  unordered_map<int, int> pageMap;
  int faults = 0;
  int counter = 0;
  int occupiedCount = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    counter++;
    cout << "访问页面 " << page << ": ";
    if (pageMap.find(page) != pageMap.end()) {
      int index = pageMap[page];
      frequency[index]++;
      timeStamp[index] = counter;
      cout << "页面已在内存中, 频率更新为 " << frequency[index];
    } else {
      faults++;
      if (occupiedCount < frames) {
        memory[occupiedCount] = page;
        frequency[occupiedCount] = 1;
        timeStamp[occupiedCount] = counter;
        pageMap[page] = occupiedCount;
        occupiedCount++;
        cout << "装入页面 " << page << " 到位置 " << occupiedCount - 1;
      } else {
        int minFreq = INT_MAX;
        int oldest = counter + 1;
        int replaceIndex = 0;
        for (int j = 0; j < frames; j++) {
          if (frequency[j] < minFreq) {
            minFreq = frequency[j];
            replaceIndex = j;
            oldest = timeStamp[j];
          } else if (frequency[j] == minFreq && timeStamp[j] < oldest) {
            replaceIndex = j;
            oldest = timeStamp[j];
          }
        }
        cout << "替换页面 " << memory[replaceIndex]
             << " (频率=" << frequency[replaceIndex] << ") 为页面 " << page
             << " (位置 " << replaceIndex << ")";
        pageMap.erase(memory[replaceIndex]);
        memory[replaceIndex] = page;
        frequency[replaceIndex] = 1;
        timeStamp[replaceIndex] = counter;
        pageMap[page] = replaceIndex;
      }
      cout << " [缺页]";
    }
    cout << " 内存状态: [";
    for (int j = 0; j < frames; j++) {
      if (j > 0)
        cout << " ";
      if (memory[j] == -1)
        cout << "-";
      else
        cout << memory[j] << "(" << frequency[j] << ")";
    }
    cout << "]" << endl;
  }
  return faults;
}
void testAlgorithm(string name, int (*algo)(vector<int> &, int),
                   vector<int> &pages, int frames) {
  cout << name << "算法:" << endl;
  cout << "页面序列: ";
  for (int page : pages) {
    cout << page << " ";
  }
  cout << endl;
  cout << "物理块数: " << frames << endl;
  int faults = algo(pages, frames);
  cout << "缺页次数: " << faults << endl;
  cout << "缺页率: " << (double)faults / pages.size() * 100 << "%" << endl;
  cout << "========================" << endl;
}
int main() {
  vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
  int frames = 3;
  cout << "页面置换算法演示" << endl;
  cout << "========================" << endl;
  testAlgorithm("OPT (最佳)", opt, pages, frames);
  testAlgorithm("FIFO (先进先出)", fifo, pages, frames);
  testAlgorithm("LRU (最近最少使用)", lru, pages, frames);
  testAlgorithm("LFU (最少使用)", lfu, pages, frames);
  return 0;
}