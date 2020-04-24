#include "mmu.h"

Mmu::Mmu(int memory_size)
{
  //  printf("Creating new mmu\n");
    _next_pid = 1024;
    _max_size = memory_size;
}

Mmu::~Mmu()
{
}

uint32_t Mmu::createProcess()
{
  //    printf("Creating new process\n");
    Process *proc = new Process();
    proc->pid = _next_pid;

    Variable *var = new Variable();
    var->name = "<FREE_SPACE>";
    var->virtual_address = 0;
    var->size = _max_size;
    var->type_size = 1;
    proc->variables.push_back(var);

    _processes.push_back(proc);
    //    std::cout<<"New process pid is "<<proc->pid<<std::endl;
    _next_pid++;
    return proc->pid;
}

Process* Mmu::getProcessFromPid(int pid)
{
	for(int i = 0; i < _processes.size(); i++)
	{
		if(_processes[i]->pid == pid)
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

void Mmu::printAllRunningProcesses()
{
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

void Mmu::printValueOfVariable(int pid, std::string var_name)
{
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
            // TODO: print all variables (excluding <FREE_SPACE> entries)
            std::cout << _processes[i]->pid << "|" << _processes[i]->variables[j]->name << "|" << _processes[i]->variables[j]->virtual_address << "|" << _processes[i]->variables[j]->size<< std::endl;
        }
    }
}
