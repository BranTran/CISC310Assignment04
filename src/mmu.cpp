#include "mmu.h"
#include <iostream> //std::hex
#include <iomanip> //std::setw
#include <string.h> //memcpy
void printDataByType(void* data, int data_size, int num_of_elements, std::string type_name);


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
    text->type_name = "char";
    proc->variables.push_back(text);
    offset = text_size;
    
    //ADD GLOBALS (data)
    Variable *data = new Variable();
    data->name = "<GLOBALS>";
    data->virtual_address = offset;
    data->size = data_size;
    data->type_size = 1;
    data->type_name = "char";
    proc->variables.push_back(data);
    offset = offset+data_size;
    
    //ADD STACK    
    Variable *stack = new Variable();
    stack->name = "<STACK>";
    stack->virtual_address = offset;
    stack->size = 65536;
    stack->type_size = 1;
    stack->type_name = "char";
    proc->variables.push_back(stack);
    offset = offset+65536;
    
    //FREE SPACE    
    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = offset;
    var->size = _max_size-offset;
    var->type_size = 1;
    var->type_name = "char";
    proc->variables.push_back(var);

    _processes.push_back(proc);
    proc->mem_offset = offset;
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

int Mmu::getVirtualAddressOfAVariable(int pid, std::string var_name)
{
	Process* process = getProcessFromPid(pid);
	for(int i = 0; i < process->variables.size(); i++)
	{
		Variable* variable = process->variables[i];
		if(variable->name.compare(var_name) == 0)
		{
			return variable->virtual_address;
		}	
	}
	return -1;
}

void Mmu::removePidFromMmu(int pid)
{
	for (int i = 0; i < _processes.size(); i++)
    {
        if(_processes[i]->pid == pid)
        {
        	_processes.erase(_processes.begin() + i);
        }   
    }
}

void Mmu::freeVariableFromProcess(int pid, std::string var_name, PageTable* pagetable)
{
    bool found;
    Process* process = getProcessFromPid(pid);
    for(int i = 0; i < process->variables.size(); i++){
        Variable* prev;
        Variable* target = process->variables[i];
        Variable* next;
        
        if(i != 0){
            prev = process->variables[i-1];
        }

        if(i < (process->variables.size() - 1))
        {
            next = process->variables[i+1];
        }
        if(found){//
//            pagetable.addEntryFromVirtualAddress(pid, target->virtual_address);
        }
        if(target->name.compare(var_name) == 0)
        {
            //Merge with next
            if(next->name.compare("<FREE_SPACE>") == 0)
            {
                target->name = "<FREE_SPACE>";
                target->size = target->size + next->size;
                target->type_size = 1;
                target->type_name = "char";
                pagetable->removeEntry(pid, next->virtual_address, next->size);
                process->variables.erase(process->variables.begin() + i + 1);//Remove next               
            }
            //Merge with prev
            if(prev->name.compare("<FREE_SPACE>") == 0)   
            {
                prev->size = prev->size + target->size;
                pagetable->removeEntry(pid, target->virtual_address, target->size);
                process->variables.erase(process->variables.begin() + i);//Removes target
            }
            found = true;
        }
        
    }
    if(!found){
        std::cout << "Error: Could not find variable " << var_name << " in process " << pid << std::endl;
    }
    
}


void Mmu::printAllRunningProcesses()
{
    for (int i = 0; i < _processes.size(); i++)
    {
        std::cout << _processes[i]->pid << std::endl;       
    }
}

void Mmu::printValueOfVariable(int pid, std::string var_name, PageTable* pagetable, uint8_t *memory)
{
    Process* process = getProcessFromPid(pid);
    int i, j;
    bool found = false;
    for (i = 0; i < process->variables.size() && !found; i++)
    {
    	if(process->variables[i]->name.compare(var_name) == 0)
    	{
    		//found variable
    		int address = pagetable->
getPhysicalAddress(pid, process->variables[i]->virtual_address);
                Variable* target = process->variables[i];
                int num_of_elements = target->size / target->type_size;
                uint8_t data[target->size];
                //Memcopy copies numBytes bytes from address to address to
                //void *to, const void *from, size_t(numbytes)
                memcpy(data,&memory[address],target->size);
                
                printDataByType(data,target->size,num_of_elements,target->type_name);
                
                found = true;
                
        }
    }
    if(!found)
    {
        std::cout << "Error: Could not find variable " << var_name << " in process " << pid << std::endl;
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

void printDataByType(void* data, int data_size, int num_of_elements, std::string type_name)
{
    bool more = (num_of_elements > 4);   
    int i,top;
    if(more){
        top = 4;        
    }
    else{
        top = num_of_elements;
    }
    //Print based on datatype   
    if(type_name.compare("char") == 0)
    {
        char* typed_data = (char*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;        
    }
    else if(type_name.compare("short") == 0)
    {
        short* typed_data = (short*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;               
    }
    else if(type_name.compare("int") == 0)
    {
        int* typed_data = (int*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;                
    }
    else if(type_name.compare("float") == 0)
    {
        float* typed_data = (float*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;                
    }
    else if(type_name.compare("long") == 0)
    {
        long* typed_data = (long*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;                
    }
    else if(type_name.compare("double") == 0)
    {
        double* typed_data = (double*) data;
        std::cout << typed_data[0];
        for(i = 1; i < top; i++)
        {
            std::cout << ", "<< typed_data[i];
        }
        if(more){
            std::cout << ", ... [" << num_of_elements << " items]";
        }
        std::cout << std::endl;                
    }         
}

