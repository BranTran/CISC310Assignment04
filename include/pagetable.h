#ifndef __PAGETABLE_H_
#define __PAGETABLE_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>

class PageTable {
private:
    int _page_size;
    std::map<std::string, int> _table;

public:
    PageTable(int page_size);
    ~PageTable();

    void addEntry(uint32_t pid, int page_number);
    int getPhysicalAddress(uint32_t pid, int virtual_address);
    void print();
};
std::vector<std::string> splitString(std::string text, char d);
#endif // __PAGETABLE_H_
