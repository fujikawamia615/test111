#include <algorithm>
#include <climits>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;
int opt(vector<int> &pages, int frames) {
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
    if (!found) {
      faults++;
      if (occupiedCount < frames) {
        memory[occupiedCount] = page;
        occupiedCount++;
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
        memory[replaceIndex] = page;
      }
    }
  }
  return faults;
}
int fifo(vector<int> &pages, int frames) {
  int n = pages.size();
  list<int> frame;
  unordered_map<int, bool> inFrame;
  int faults = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    if (!inFrame[page]) {
      faults++;
      if (frame.size() == frames) {
        int removed = frame.front();
        frame.pop_front();
        inFrame[removed] = false;
      }
      frame.push_back(page);
      inFrame[page] = true;
    }
  }
  return faults;
}
int lru(vector<int> &pages, int frames) {
  int n = pages.size();
  list<int> frame;
  unordered_map<int, list<int>::iterator> pagePos;
  int faults = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    if (pagePos.find(page) == pagePos.end()) {
      faults++;
      if (frame.size() == frames) {
        int removed = frame.front();
        frame.pop_front();
        pagePos.erase(removed);
      }
    } else {
      frame.erase(pagePos[page]);
    }
    frame.push_back(page);
    pagePos[page] = --frame.end();
  }
  return faults;
}
int lfu(vector<int> &pages, int frames) {
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
    if (pageMap.find(page) != pageMap.end()) {
      int index = pageMap[page];
      frequency[index]++;
      timeStamp[index] = counter;
      continue;
    }
    faults++;
    if (occupiedCount < frames) {
      memory[occupiedCount] = page;
      frequency[occupiedCount] = 1;
      timeStamp[occupiedCount] = counter;
      pageMap[page] = occupiedCount;
      occupiedCount++;
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
      pageMap.erase(memory[replaceIndex]);
      memory[replaceIndex] = page;
      frequency[replaceIndex] = 1;
      timeStamp[replaceIndex] = counter;
      pageMap[page] = replaceIndex;
    }
  }
  return faults;
}
void testAlgorithm(string name, int (*algo)(vector<int> &, int),
                   vector<int> &pages, int frames) {
  int faults = algo(pages, frames);
  cout << name << "算法:" << endl;
  cout << "页面序列: ";
  for (int page : pages) {
    cout << page << " ";
  }
  cout << endl;
  cout << "物理块数: " << frames << endl;
  cout << "缺页次数: " << faults << endl;
  cout << "缺页率: " << (double)faults / pages.size() * 100 << "%" << endl;
  cout << "------------------------" << endl;
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