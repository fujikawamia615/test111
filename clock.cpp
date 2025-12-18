#include <iostream>
#include <list>
#include <vector>
using namespace std;
int clock(vector<int> &pages, int frames) {
  int n = pages.size();
  vector<int> memory(frames, -1);
  vector<bool> reference(frames, false);
  int faults = 0;
  int pointer = 0;
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    bool found = false;
    for (int j = 0; j < frames; j++) {
      if (memory[j] == page) {
        found = true;
        reference[j] = true;
        break;
      }
    }
    if (!found) {
      faults++;
      while (reference[pointer]) {
        reference[pointer] = false;
        pointer = (pointer + 1) % frames;
      }
      memory[pointer] = page;
      reference[pointer] = true;
      pointer = (pointer + 1) % frames;
    }
  }
  return faults;
}
int enhanced_clock(vector<int> &pages, int frames) {
  int n = pages.size();
  vector<int> memory(frames, -1);
  vector<bool> reference(frames, false);
  vector<bool> modified(frames, false);
  int faults = 0;
  int pointer = 0;
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    bool found = false;
    for (int j = 0; j < frames; j++) {
      if (memory[j] == page) {
        found = true;
        reference[j] = true;
        if (rand() % 2 == 0) {
          modified[j] = true;
        }
        break;
      }
    }
    if (!found) {
      faults++;
      int scan_count = 0;
      bool page_replaced = false;
      while (!page_replaced) {
        scan_count++;
        switch (scan_count) {
        case 1:
          for (int j = 0; j < frames; j++) {
            if (!reference[j] && !modified[j]) {
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
          }
          break;
        case 2:
          for (int j = 0; j < frames; j++) {
            if (!reference[j] && modified[j]) {
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
            reference[j] = false;
          }
          break;
        case 3:
          for (int j = 0; j < frames; j++) {
            if (!modified[j] && !reference[j]) {
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
          }
          break;
        case 4:
          for (int j = 0; j < frames; j++) {
            if (modified[j] && !reference[j]) {
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
          }
          break;
        }
      }
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
  cout << "Clock页面置换算法演示" << endl;
  cout << "========================" << endl;
  testAlgorithm("Clock", clock, pages, frames);
  testAlgorithm("Enhanced Clock", enhanced_clock, pages, frames);
  return 0;
}