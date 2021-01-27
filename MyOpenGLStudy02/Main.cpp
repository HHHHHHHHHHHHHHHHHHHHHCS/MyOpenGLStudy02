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

	//����д���������л���
	DoMain();
	
	//����1xx ��й¶���ù�
	_CrtDumpMemoryLeaks();
	
	return 0;
}



