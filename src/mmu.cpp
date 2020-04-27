#include "mmu.h"
#include <iostream> //std::hex
#include <iomanip> //std::setw
Mmu::Mmu(int memory_size)
{
  //  printf("Creating new mmu\n");
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess(int text_size, int data_size)
{
    int offset = 0;
    
  //    printf("Creating new process\n");
    Process *proc = new Process();
    proc->pid = _next_pid;
    //Add TEXT
    Variable *text = new Variable();
    text->name = "<TEXT>";
    text->virtual_address = offset;
    text->size = text_size;
    text->type_size = 1;
    proc->variables.push_back(text);
    offset = text_size;
    
    //ADD GLOBALS (data)
    Variable *data = new Variable();
    data->name = "<GLOBALS>";
    data->virtual_address = offset;
    data->size = data_size;
    data->type_size = 1;
    proc->variables.push_back(data);
    offset = offset+data_size;
    
    //ADD STACK    
    Variable *stack = new Variable();
    stack->name = "<STACK>";
    stack->virtual_address = offset;
    stack->size = 65536;
    stack->type_size = 1;
    proc->variables.push_back(stack);
    offset = offset+65536;
    
    //FREE SPACE    
    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = offset;
    var->size = _max_size-offset;
    var->type_size = 1;
    proc->variables.push_back(var);

    _processes.push_back(proc);
    //    std::cout<<"New process pid is "<<proc->pid<<std::endl;
    _next_pid++;
    return proc->pid;
}

Process* Mmu::getProcessFromPid(int processID)
{
	for(int i = 0; i < _processes.size(); i++)
	{
		if(_processes[i]->pid == processID)
		{
                    printf("We got a valid process ID back: %d\n", processID);
                    return _processes[i];
		}
	}
	return NULL;
}

Variable* Mmu::getFreeSpace(int size, Process* process)
{
	for(int i = 0; i < process->variables.size(); i++)
	{
		Variable* variable = process->variables[i];
		if(variable->name.compare("<FREE_SPACE>"))
		{
			if(variable->size >= size)//potential paging issues
			{
				return variable;
			}
		}	
	}
	return NULL;
}

void Mmu::printAllRunningProcesses()
{
    for (int i = 0; i < _processes.size(); i++)
    {
        std::cout << _processes[i]->pid << std::endl;       
    }
}

void Mmu::printValueOfVariable(int pid, std::string var_name)
{
    //NEED TO EDIT
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            std::cout << _processes[i]->pid << "|" << _processes[i]->variables[j]->name << "|" << _processes[i]->variables[j]->virtual_address << "|" << _processes[i]->variables[j]->size<< std::endl;
        }
    }
}

void Mmu::print()
{
    int i, j;

    std::cout << " PID  | Variable Name | Virtual Addr | Size" << std::endl;
    std::cout << "------+---------------+--------------+------------" << std::endl;
    for (i = 0; i < _processes.size(); i++)
    {
        for (j = 0; j < _processes[i]->variables.size(); j++)
        {
            if(_processes[i]->variables[j]->name.compare("<FREE_SPACE>") != 0){
                
                // TODO: print all variables (excluding <FREE_SPACE> entries)
                //PID
                std::cout << std::setw(5) << _processes[i]->pid << " | ";
                //Variable name
                std::cout << std::left <<std::setw(13) << _processes[i]->variables[j]->name << std::right << " | ";
                std::cout << "  0x" << std::setfill('0') << std::hex << std::uppercase << std::setw(8) << _processes[i]->variables[j]->virtual_address << std::setfill(' ') << std::dec << " | ";
                std::cout << std::setw(11) << _processes[i]->variables[j]->size<< std::endl;
            }
            
        }
    }
}
