#include <iostream>
#include <string>
#include "mmu.h"
#include "pagetable.h"

#define MEMORYSIZE 67108864
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
      fprintf(stderr, "Error: entered page size is not a number: %s\n",size_input);
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
      fprintf(stderr, "Error: entered page size is not a valid power of two [1024,32768]: %d\n",page_size);
      return 1;
    }
      
    printStartMessage(page_size);

    // Create physical 'memory'
    uint8_t *memory = new uint8_t[MEMORYSIZE]; // 64 MB (64 * 1024 * 1024)
    //Create mmu
    Mmu mmu = Mmu(MEMORYSIZE);
    //Create page table
    PageTable pagetable = PageTable(page_size);
    // Prompt loop
    std::string command;
    std::cout << "> ";
    std::getline (std::cin, command);
    char* argument_name;
    std::string argument;
    uint32_t pid;
    while (command != "exit") {
        // Handle command
        // TODO: implement this!
      bool isValid = true;
      bool isVerified = true;
        // Parse the command on spaces (from assignment 2)
      std::vector<std::string> argv = splitString(command,' ');
      int argc = argv.size();
      argument = argv[0];
      argument_name = const_cast<char*>(argument.c_str());
      
        // create <text_size> <data_size>
      if(argument.compare("create") == 0){
	//Verify create input parameters
	if(argc!=3){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <text_size> <data_size>\n",argument_name,argument_name);
	  bool isVerified = false;
	}
	if(isVerified)
	  //Verify create inputs
	  char* text_try = const_cast<char*>(argv[1].c_str());
	  char* data_try = const_cast<char*>(argv[2].c_str());
	  
	  if(!isNonNegativeInteger(text_try)){
	    fprintf(stderr, "Error: entered text_size is not a number: %s\n",text_try);
	    isVerified = false;
	  }
	  else{
	    int text_size = atoi(text_try);
	  }
	  if(!isNonNegativeInteger(data_try)){
	    fprintf(stderr, "Error: entered data_size is not a number: %s\n",data_try);
	    isVerified = false;
	  }
	  else{
	    int data_size = atoi(data_try);
	  }
	    
	  if(isVerified){
	    //Validate
	    if(text_size < 2048 || text_size > 16384){
	      fprintf(stderr, "Error: entered text_size is not in range [2048,16384]: %d\n",text_size);
	      isValid = false;
	    }
	    if(data_size < 0 || data_size > 1024){
	      fprintf(stderr, "Error: entered data_size is not in range [0,1024]: %d\n",data_size);
	      isValid = false;
	    }
	    if(isValid){
	      // GOOD TO GO - DO THE THING
	      //Create the new process
	      pid = mmu.createProcess();
	      //Print the PID
	      std::cout<< pid <<std::endl;
	    }
	  }
	}
      }
        // allocate <PID> <var_name> <data_type> <number of elements>
      else if(argument.compare("allocate") == 0){
	if(argc!=5){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_name> <data_type> <number of elements>\n",argument_name,argument_name);
	  isVerfied = false;
	}
	if(isVerified){
	  char* pid_try = const_cast<char*>(argv[1].c_str());
	  std::string var_name = argv[2];
	  std::string data_type = argv[3];
	  char* num_try = const_cast<char*>(argv[4].c_str());
	  int var_size;
	  
	  //Proper PID
	  if(!isNonNegativeInteger(pid_try)){
	    fprintf(stderr, "Error: entered PID is not a number: %s\n",pid_try);
	    isVerified = false;
	  }
	  else{
	    int pid = atoi(pid_try);
	  }
	  //proper data_type
	  if(data_type.compare("char")){
	    var_size = 1;
	  }
	  else if(data_type.compare("short")){
	    var_size = 2;
	  }
	  else if(data_type.compare("int")){
	    var_size = 4;
	  }
	  else if(data_type.compare("float")){
	    var_size = 4;
	  }
	  else if(data_type.compare("long")){
	    var_size = 8;
	  }
	  else if(data_type.compare("double")){
	    var_size = 8;
	  }
	  else{
	    fprintf(stderr, "Error: entered data_type is not recognized %s\nList of Valid Datatypes:\nchar\nshort\nint\nfloat\nlong\ndouble\n",num_try);
	    isVerified = false;
	  }
	  //proper num_elements
	  if(!isNonNegativeInteger(num_try)){
	    fprintf(stderr, "Error: entered number of elements is not a number: %s\n",num_try);
	    isVerified = false;
	  }
	  else{
	    int num_elements = atoi(num_try);
	  }
	  
	  if(isVerified){

	    //VALIDATE DATA
	    // Does the PID exists
	    // is the var_name a new variable
	    // do we have enough size
	    if(isValid){
	      // GOOD TO GO - DO THE THING
	    }
	    
	  }
	  
	}
	
      }
        // set <PID> <var_name> <offset> <value_0> <value_1> ... <value_N>
      else if(argument.compare("set") == 0){
	if(argc < 5){
	  fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_name> <offset> <value_0> [<value_1> ...]\n",argument_name,argument_name);
	  return 1;
	}
      }
    // free <PID> <var_name>
	else if(argument.compare("free") == 0)
	{
		int argumentOk = 0;
		if(argc!=3)
		{
			fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID> <var_name>\n",argument_name,argument_name);
			argumentOk++;
		}
		else
		{
			char* pid_try = const_cast<char*>(argv[1].c_str());
			char* var_name_try = const_cast<char*>(argv[2].c_str());
			if(!isNonNegativeInteger(pid_try))
			{
				fprintf(stderr, "Error: entered PID is not a number: %s\n",pid_try);
				argumentOk++;
			}
			if(false)//ERROR CHECK var_name
			{
				argumentOk++;
			}
			if(argumentOk == 0)
			{
				int pid = atoi(pid_try);
				isVerified = true;
			}
		}
	}
    // terminate <PID>
    else if(argument.compare("terminate") == 0)
    {
		int argumentOk = 0;
    	if(argc!=2)
    	{
    		fprintf(stderr,"Error: %s does not have the right number of arguments: %s <PID>\n",argument_name,argument_name);
    		argumentOk++;
		}
		else
		{
			char* pid_try = const_cast<char*>(argv[1].c_str());
			if(!isNonNegativeInteger(pid_try))
			{
				fprintf(stderr, "Error: entered PID is not a number: %s\n",pid_try);
				argumentOk++;
			}
			if(argumentOk == 0)
			{
				int pid = atoi(pid_try);
				isVerified = true;
			}
		}
	}
    // print <object>
	else if(argument.compare("print") == 0)
	{
		int argumentOk = 0;
		if(argc!=2)
		{
			fprintf(stderr,"Error: %s does not have the right number of arguments: %s <object>\n",argument_name,argument_name);
			argumentOk++;
		}
		else
		{
			char* object_try_num = const_cast<char*>(argv[1].c_str()); 
			std::string object_try = argv[1].c_str();
			if(object_try.compare("mmu") != 0 && object_try.compare("page") != 0 && object_try.compare("processes") != 0 && object_try.find(":") == -1)
			{
				fprintf(stderr, "Error: entered object is not valid: %s (accepted values: 'mmu', 'page', 'processes' or a '<PID>:<var_name>\n",object_try_num);
				argumentOk++;
			}
			else if(object_try.compare("mmu") == 0 || object_try.compare("page") == 0 || object_try.compare("processes") == 0)
			{
			}
			else
			{
				std::vector<std::string> print_pid = splitString(object_try,':');
				if(print_pid.size() != 2)
				{
					fprintf(stderr, "Error: entered object is not valid: %s (accepted values: 'mmu', 'page', 'processes' or a '<PID>:<var_name>\n",object_try_num);
					argumentOk++;
				}
				else
				{
					char* pid_try = const_cast<char*>(print_pid[0].c_str());
					char* var_name_try = const_cast<char*>(print_pid[1].c_str());
					if(!isNonNegativeInteger(pid_try))
					{
						fprintf(stderr, "Error: entered object is not valid: %s (accepted values: 'mmu', 'page', 'processes' or a '<PID>:<var_name>\n",object_try_num);
						argumentOk++;
					}
					if(false) //ERROR CHECK var_name
					{
						argumentOk++;
					}
				}
			}
			if(argumentOk == 0)
			{
				isVerified = true;
			}
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
