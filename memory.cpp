#include <algorithm>
#include <iostream>
#include <list>
#include <stdlib.h>
#include <vector>
using namespace std;
typedef struct block1 {
  int start, size;
} fre;
typedef struct block2 {
  int start, size, id;
} used;
struct PrintBlock {
  int start;
  int size;
  int id;
  bool isFree;
  bool operator<(const PrintBlock &other) const { return start < other.start; }
};
list<fre> freelist;
list<used> usedlist;
void insertbyadd(int start, int size) {
  fre newblock = {start, size};
  if (freelist.empty()) {
    freelist.push_back(newblock);
    return;
  }
  for (list<fre>::iterator it = freelist.begin(); it != freelist.end(); ++it) {
    if (newblock.start < it->start) {
      freelist.insert(it, newblock);
      return;
    }
  }
  freelist.push_back(newblock);
}
void insertbysizeup(int start, int size) {
  fre newblock = {start, size};
  if (freelist.empty()) {
    freelist.push_back(newblock);
    return;
  }
  for (list<fre>::iterator it = freelist.begin(); it != freelist.end(); ++it) {
    if (newblock.size < it->size ||
        (newblock.size == it->size && newblock.start < it->start)) {
      freelist.insert(it, newblock);
      return;
    }
  }
  freelist.push_back(newblock);
}
void insertbysizedown(int start, int size) {
  fre newblock = {start, size};
  if (freelist.empty()) {
    freelist.push_back(newblock);
    return;
  }
  for (list<fre>::iterator it = freelist.begin(); it != freelist.end(); ++it) {
    if (newblock.size > it->size ||
        (newblock.size == it->size && newblock.start < it->start)) {
      freelist.insert(it, newblock);
      return;
    }
  }
  freelist.push_back(newblock);
}
void back(int id, int size, void (*insert)(int, int)) {
  for (list<used>::iterator it = usedlist.begin(); it != usedlist.end(); ++it) {
    if ((*it).id == id && (*it).size == size) {
      int releasedstart = (*it).start;
      int releasedsize = (*it).size;
      usedlist.erase(it);
      fre newfree = {releasedstart, releasedsize};
      list<fre>::iterator left_it = freelist.end();
      list<fre>::iterator right_it = freelist.end();
      for (list<fre>::iterator it1 = freelist.begin(); it1 != freelist.end();
           ++it1) {
        if (it1->start + it1->size == newfree.start) {
          left_it = it1;
        } else if (newfree.start + newfree.size == it1->start) {
          right_it = it1;
        }
      }
      int finalstart = newfree.start;
      int finalsize = newfree.size;
      if (left_it != freelist.end() && right_it != freelist.end()) {
        finalstart = left_it->start;
        finalsize = right_it->start + right_it->size - left_it->start;
        freelist.erase(left_it);
        freelist.erase(right_it);
      } else if (left_it != freelist.end()) {
        finalstart = left_it->start;
        finalsize = left_it->size + newfree.size;
        freelist.erase(left_it);
      } else if (right_it != freelist.end()) {
        finalstart = newfree.start;
        finalsize = newfree.size + right_it->size;
        freelist.erase(right_it);
      }
      insert(finalstart, finalsize);
      break;
    }
  }
}
void backbyadd(int id, int size) { back(id, size, insertbyadd); }
void backbysizeup(int id, int size) { back(id, size, insertbysizeup); }
void backbysizedown(int id, int size) { back(id, size, insertbysizedown); }
bool fit(int id, int size, void (*insert)(int, int)) {
  if (size <= 0)
    return false;
  if (freelist.empty())
    return false;
  for (list<fre>::iterator it = freelist.begin(); it != freelist.end(); ++it) {
    if ((*it).size >= size) {
      used use1 = {(*it).start, size, id};
      usedlist.push_back(use1);
      if ((*it).size == size) {
        freelist.erase(it);
      } else {
        int remainstart = (*it).start + size;
        int remainsize = (*it).size - size;
        freelist.erase(it);
        insert(remainstart, remainsize);
      }
      return true;
    }
  }
  return false;
}
bool firstfit(int id, int size) { return fit(id, size, insertbyadd); }
list<fre>::iterator nextfitptr = freelist.begin();
bool nextfit(int id, int size) {
  if (size <= 0)
    return false;
  if (freelist.empty())
    return false;
  list<fre>::iterator originposition = nextfitptr;
  bool firsttime = true;
  while (true) {
    if (nextfitptr == freelist.end()) {
      nextfitptr = freelist.begin();
      firsttime = false;
    }
    if ((*nextfitptr).size >= size) {
      used use1 = {(*nextfitptr).start, size, id};
      usedlist.push_back(use1);
      if ((*nextfitptr).size == size) {
        list<fre>::iterator toerase = nextfitptr;
        ++nextfitptr;
        freelist.erase(toerase);
      } else {
        int remainstart = (*nextfitptr).start + size;
        int remainsize = (*nextfitptr).size - size;
        list<fre>::iterator toerase = nextfitptr;
        ++nextfitptr;
        freelist.erase(toerase);
        insertbyadd(remainstart, remainsize);
      }
      return true;
    }
    ++nextfitptr;
    if (!firsttime && nextfitptr == originposition)
      break;
  }
  return false;
}
bool bestfit(int id, int size) { return fit(id, size, insertbysizeup); }
bool worstFit(int id, int size) { return fit(id, size, insertbysizedown); }
void printmemory() {
  vector<PrintBlock> allBlocks;
  for (list<fre>::iterator it = freelist.begin(); it != freelist.end(); ++it) {
    PrintBlock pb;
    pb.start = it->start;
    pb.size = it->size;
    pb.id = -1;
    pb.isFree = true;
    allBlocks.push_back(pb);
  }
  for (list<used>::iterator it = usedlist.begin(); it != usedlist.end(); ++it) {
    PrintBlock pb;
    pb.start = it->start;
    pb.size = it->size;
    pb.id = it->id;
    pb.isFree = false;
    allBlocks.push_back(pb);
  }
  sort(allBlocks.begin(), allBlocks.end());
  cout << "内存状态 (按起始地址排序): ";
  for (vector<PrintBlock>::iterator it = allBlocks.begin();
       it != allBlocks.end(); ++it) {
    if (it->isFree) {
      cout << "[未使用: start=" << it->start << ", size=" << it->size << "] ";
    } else {
      cout << "[已使用 ID=" << it->id << ": start=" << it->start
           << ", size=" << it->size << "] ";
    }
  }
  cout << endl;
}
void runTest(const char *functionname, bool (*dofun)(int, int),
             void (*dedofun)(int, int)) {
  cout << "\n" << functionname << " 算法测试" << endl;
  freelist.clear();
  usedlist.clear();
  fre initialblock = {0, 512};
  freelist.push_back(initialblock);
  cout << boolalpha;
  cout << "初始状态: ";
  printmemory();
  cout << "\nP1申请300KB: " << dofun(1, 300) << endl;
  cout << "当前状态: ";
  printmemory();
  cout << "\nP2申请100KB: " << dofun(2, 100) << endl;
  cout << "当前状态: ";
  printmemory();
  dedofun(1, 300);
  cout << "\nP1释放300KB: " << endl;
  cout << "当前状态: ";
  printmemory();
  cout << "\nP3申请150KB: " << dofun(3, 150) << endl;
  cout << "当前状态: ";
  printmemory();
  cout << "\nP4申请30KB: " << dofun(4, 30) << endl;
  cout << "当前状态: ";
  printmemory();
  cout << "\nP5申请40KB: " << dofun(5, 40) << endl;
  cout << "当前状态: ";
  printmemory();
  cout << "\nP6申请60KB: " << dofun(6, 60) << endl;
  cout << "当前状态: ";
  printmemory();
  dedofun(4, 30);
  cout << "\n释放P4的30KB: " << endl;
  cout << "最终状态: ";
  printmemory();
}
int main() {
  runTest("首次适应", firstfit, backbyadd);
  runTest("最佳适应", bestfit, backbysizeup);
  runTest("最坏适应", worstFit, backbysizedown);
  runTest("循环首次适应", nextfit, backbyadd);
  return 0;
}