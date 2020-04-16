#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"

void printStartMessage(int page_size);
std::vector<std::string> splitString(std::string text, char d);
bool isNonNegativeInteger(char* pointer);

int main(int argc, char **argv)
{
    // Ensure user specified page size as a command line parameter
    if (argc < 2)
    {
        fprintf(stderr, "Error: you must specify the page size\n");
        return 1;
    }

    // Print opening instuction message
    // int page_size = std::stoi(argv[1]);
    //-Make sure it is a number (assignment 1)
    char* size_input;
    int page_size;
    size_input = argv[1];
    if(!isNonNegativeInteger(size_input)){
      fprintf(stderr, "Error: entered page size is not a number %s\n",size_input);
      return 1;
    }
    
    page_size = atoi(size_input);
    //-Make sure that it is a power of 2 up to 32768
    int power_two = 1024;
    while(page_size != power_two && power_two <= 32768){
      power_two = power_two << 1;//Next power of two
      //printf("Power of two is: %d\n", power_two);
    }
    if(power_two > 32768 || page_size != power_two){
      fprintf(stderr, "Error: entered page size is not a power of two %d\n",page_size);
      return 1;
    }
      
    printStartMessage(page_size);

    // Create physical 'memory'
    uint8_t *memory = new uint8_t[67108864]; // 64 MB (64 * 1024 * 1024)

    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    while (command != "exit") {
        // Handle command
        // TODO: implement this!
        // Parse the command on spaces (from assignment 2)
      std::vector<std::string> argv = splitString(command,' ');
      int argc = argv.size();
      std::string argument;
      argument = argv[0];
      
        // create <text_size> <data_size>
      if(argument.compare("create") == 0){
	//Verify create input parameters
	if(argc!=3){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <text_size> <data_size>\n",argument,argument);
	}
	//Validate create inputs
	char* text_try = const_cast<char*>(argv[1].c_str());
	if(!isNonNegativeInteger(text_try)){
	  fprintf(stderr, "Error: entered text_size is not a number %s\n",text_try);
	  return 1;
	}
	int text_size = atoi(text_try);
	//int data_size = atoi(argv[2]);
      }
        // allocate <PID> <var_name> <data_type> <number of elements>
      else if(argument.compare("allocate") == 0){
	if(argc!=5){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_size> <data_type> <number of elements>\n",argument,argument);
	}
      }
        // set <PID> <var_name> <offset> <value_0> <value_1> ... <value_N>
      else if(argument.compare("set") == 0){
	if(argc < 5){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_name> <offset> <value_0> [<value_1> ...]\n",argument,argument);
	}
      }
        // free <PID> <var_name>
      else if(argument.compare("free") == 0){
	if(argc!=3){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_name>\n",argument,argument);
	}
      }
        // terminate <PID>
      else if(argument.compare("terminate") == 0){
	if(argc!=2){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID>\n",argument,argument);
	}
      }
        // print <object>
      else if(argument.compare("print") == 0){
	if(argc!=2){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <object>\n",argument,argument);
	}
      }
        // otherwise send an error
      else{
	printf("Error: command not found %s\n",command.c_str());
      }
        // Get next command
        std::cout << "> ";
        std::getline (std::cin, command);
    }


    return 0;
}

void printStartMessage(int page_size)
{
    std::cout << "Welcome to the Memory Allocation Simulator! Using a page size of " << page_size << " bytes." << std:: endl;
    std::cout << "Commands:" << std:: endl;
    std::cout << "  * create <text_size> <data_size> (initializes a new process)" << std:: endl;
    std::cout << "  * allocate <PID> <var_name> <data_type> <number_of_elements> (allocated memory on the heap)" << std:: endl;
    std::cout << "  * set <PID> <var_name> <offset> <value_0> <value_1> <value_2> ... <value_N> (set the value for a variable)" << std:: endl;
    std::cout << "  * free <PID> <var_name> (deallocate memory on the heap that is associated with <var_name>)" << std:: endl;
    std::cout << "  * terminate <PID> (kill the specified process)" << std:: endl;
    std::cout << "  * print <object> (prints data)" << std:: endl;
    std::cout << "    * If <object> is \"mmu\", print the MMU memory table" << std:: endl;
    std::cout << "    * if <object> is \"page\", print the page table" << std:: endl;
    std::cout << "    * if <object> is \"processes\", print a list of PIDs for processes that are still running" << std:: endl;
    std::cout << "    * if <object> is a \"<PID>:<var_name>\", print the value of the variable for that process" << std:: endl;
    std::cout << std::endl;
}

bool isNonNegativeInteger(char* pointer){
  if(std::string(std::to_string(atoi(pointer))).length() < std::string(pointer).length() || pointer == NULL || !isdigit(pointer[0])){
    return false;
  }
  return true;
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