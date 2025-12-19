#include <iostream>
#include <list>
#include <vector>
using namespace std;
int clock(vector<int> &pages, int frames) {
  cout << "Clock算法执行过程:" << endl;
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
    
    cout << "访问页面 " << page << ": ";
    if (!found) {
      faults++;
      cout << "需要置换, ";
      while (reference[pointer]) {
        cout << "跳过页面 " << memory[pointer] << "(引用位=1), ";
        reference[pointer] = false;
        pointer = (pointer + 1) % frames;
      }
      cout << "置换页面 " << memory[pointer] << " 为页面 " << page;
      memory[pointer] = page;
      reference[pointer] = true;
      pointer = (pointer + 1) % frames;
      cout << " [缺页]";
    } else {
      cout << "页面已在内存中";
    }
    
    cout << " 内存状态: [";
    for (int j = 0; j < frames; j++) {
      if (j > 0) cout << " ";
      if (memory[j] == -1) 
        cout << "-";
      else 
        cout << memory[j] << "(" << reference[j] << ")";
    }
    cout << "] 指针位置: " << ((pointer-1+frames)%frames) << endl;
  }
  return faults;
}
int enhanced_clock(vector<int> &pages, int frames) {
  cout << "Enhanced Clock算法执行过程:" << endl;
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
    
    cout << "访问页面 " << page << ": ";
    if (!found) {
      faults++;
      cout << "需要置换, ";
      int scan_count = 0;
      bool page_replaced = false;
      while (!page_replaced) {
        scan_count++;
        switch (scan_count) {
        case 1:
          cout << "第一轮扫描寻找(0,0), ";
          for (int j = 0; j < frames; j++) {
            if (!reference[j] && !modified[j]) {
              cout << "找到页面 " << memory[j] << ", ";
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
          }
          break;
        case 2:
          cout << "第二轮扫描寻找(0,1), ";
          for (int j = 0; j < frames; j++) {
            if (!reference[j] && modified[j]) {
              cout << "找到页面 " << memory[j] << ", ";
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
          cout << "第三轮扫描寻找(0,0), ";
          for (int j = 0; j < frames; j++) {
            if (!modified[j] && !reference[j]) {
              cout << "找到页面 " << memory[j] << ", ";
              memory[j] = page;
              reference[j] = true;
              modified[j] = false;
              page_replaced = true;
              break;
            }
          }
          break;
        case 4:
          cout << "第四轮扫描寻找(0,1), ";
          for (int j = 0; j < frames; j++) {
            if (modified[j] && !reference[j]) {
              cout << "找到页面 " << memory[j] << ", ";
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
      cout << "置换为页面 " << page;
      cout << " [缺页]";
    } else {
      cout << "页面已在内存中";
    }
    
    cout << " 内存状态: [";
    for (int j = 0; j < frames; j++) {
      if (j > 0) cout << " ";
      if (memory[j] == -1) 
        cout << "-";
      else 
        cout << memory[j] << "(" << reference[j] << "," << modified[j] << ")";
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
  cout << "Clock页面置换算法演示" << endl;
  cout << "========================" << endl;
  testAlgorithm("Clock", clock, pages, frames);
  testAlgorithm("Enhanced Clock", enhanced_clock, pages, frames);
  return 0;
}