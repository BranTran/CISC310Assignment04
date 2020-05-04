#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>
#include <pagetable.h>

typedef struct Variable {
    std::string name;
    int virtual_address;
    int size;
    int type_size;
    std::string type_name;
} Variable;

typedef struct Process {
    uint32_t pid;
    std::vector<Variable*> variables;
    //number of pages?    
    int mem_offset;
} Process;

class Mmu {
private:
    uint32_t _next_pid;
    int _max_size;
    std::vector<Process*> _processes;


public:
    Mmu(int memory_size);
    ~Mmu();
	// we probably want to get a process 
    uint32_t createProcess(int text_size, int data_size, PageTable* pagetable);
    Process* getProcessFromPid(uint32_t pid);
    Variable* getFreeSpace(int size, Process* process);
    int getVirtualAddressOfAVariable(uint32_t pid, std::string var_name);
    
    void removePidFromMmu(uint32_t pid);
    void freeVariableFromProcess(uint32_t pid, std::string var_name, PageTable* pagetable);
    
    void print();
    void printAllRunningProcesses();
    void printValueOfVariable(uint32_t pid, std::string var_name, PageTable* pagetable, uint8_t *memory);
};

#endif // __MMU_H_
