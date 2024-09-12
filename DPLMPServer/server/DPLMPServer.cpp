// DPLMPServer.cpp : Defines the entry point for the application.
//

#include "DPLMPServer.h"
#include "Core.h"

using namespace std;

int main()
{
	Core::Initialize();
	while (true) {
		Core::Step();
	}
	return 0;
}
