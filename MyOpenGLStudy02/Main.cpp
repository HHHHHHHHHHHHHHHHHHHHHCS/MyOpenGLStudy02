#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "Program.h"

void DoMain()
{
	Program program{};
	program.DoMain();
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	  // _CrtSetBreakAlloc(3476);
	
	//单独写个函数进行回收
	DoMain();
	
	//正常1xx 2xx   < 1000 的泄露不用管
	_CrtDumpMemoryLeaks();
	
	return 0;
}



