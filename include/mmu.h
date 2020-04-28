#ifndef __MMU_H_
#define __MMU_H_

#include <iostream>
#include <string>
#include <vector>

typedef struct Variable {
    std::string name;
    int virtual_address;
    int size;
    int type_size;
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
    uint32_t createProcess(int text_size, int data_size);
    Process* getProcessFromPid(int pid);
    Variable* getFreeSpace(int size, Process* process);
    void printAllRunningProcesses();
    void printValueOfVariable(int pid, std::string var_name);
    int getVirtualAddressOfAVariable(int pid, std::string var_name);
    void print();
};

#endif // __MMU_H_
