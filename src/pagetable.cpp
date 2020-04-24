#include "pagetable.h"
#include <string>
#include <vector>

std::vector<std::string> splitString(std::string text, char d);

PageTable::PageTable(int page_size)
{
    _page_size = page_size;
}

PageTable::~PageTable()
{
}

void PageTable::addEntry(uint32_t pid, int page_number)
{
    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);

    // Find free frame
    // TODO: implement this!
    int frame = 0;
    bool frameNotFound = true;
    bool isDone = false;
    while(!isDone)
    {
    	frameNotFound = true;
    	//itereate over entire _table
    	//if(_table[key] == frame){frameNotFound = false;}
    	//if no free frames then do something
    	frame++;
    	if(frameNotFound)
    	{
    		isDone = true;
    	}
    }printf("%s into frame %d\n", entry, frame);
    _table[entry] = frame;  
}

int PageTable::getPhysicalAddress(uint32_t pid, int virtual_address)
{
    // Convert virtual address to page_number and page_offset
    // TODO: implement this!
    int page_number = 0;
    int page_offset = 0;
    page_offset = virtual_address % _page_size;
    page_number = virtual_address / _page_size;

    // Combination of pid and page number act as the key to look up frame number
    std::string entry = std::to_string(pid) + "|" + std::to_string(page_number);
    
    // If entry exists, look up frame number and convert virtual to physical address
    int address = -1;
    if (_table.count(entry) > 0)
    {
        // TODO: implement this!
        address = _table[entry];
    }

    return address;
}

void PageTable::print()
{
    std::map<std::string, int>::iterator it;

    std::cout << " PID  | Page Number | Frame Number" << std::endl;
    std::cout << "------+-------------+--------------" << std::endl;

    for (it = _table.begin(); it != _table.end(); it++)
    {
      std::string entry = (*it).first;
      
      std::vector<std::string> pid_pagenum = splitString(entry,'|');
      std::string pid = pid_pagenum[0];
      std::string pagenum = pid_pagenum[1];

      int frame = (*it).second;
      std::cout<<pid<<"|"<<pagenum<<"|"<<frame<<std::endl;
    }
}

std::vector<std::string> splitString(std::string text, char d)
{
  std::vector<std::string> result;
  int found = text.find_first_of(d,0);
  std::string before;
  std::string after;
  before = text.substr(0, found);
  after = text.substr(found+1, text.length());
  while(found != std::string::npos)
    {
      result.push_back(before);
      found = after.find_first_of(d,0);
      before = after.substr(0, found);
      after = after.substr(found+1, text.length());
    }
  result.push_back(after);
  return result;
}
