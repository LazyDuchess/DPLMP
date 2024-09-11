#include "../framework.h"
#include "Core.h"
#include "Hooking.h"
#include "ClientController.h"
#include <iostream>

namespace Core {
	void __declspec(naked) ReturnEarly() {
		__asm {
			mov eax, 0x0
			ret
		}
	}

	void __declspec(naked) ReturnEarly4() {
		__asm {
			mov eax, 0x0
			ret 0x4
		}
	}

	void __declspec(naked) AIVehicleManagerHook() {
		__asm {
			mov eax, 0x00412C2D
			jmp eax
		}
	}

	void __declspec(naked) CreateCopVehicleHook() {
		__asm {
			mov eax, 0x0040FE33
			jmp eax
		}
	}

	void __declspec(naked) CreatePedsHook() {
		__asm {
			mov eax, 0x00448D71
			jmp eax
		}
	}

	void __declspec(naked) SetGameSpeedMultiplierHook() {
		__asm {
			mov dword ptr [esp + 04], 0x3f800000
			movss xmm0, [esp + 04]
			mov eax, 0x0045ACEE
			jmp eax
		}
	}

	void Initialize() {
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);

		// REMOVE TRAFFIC HOOKS
		// 
		// createtrafficpedestrian?
		Hooking::MakeJMP((BYTE*)0x00448270, (DWORD)ReturnEarly4, 5);
		// createmulitplepedestrians?
		Hooking::MakeJMP((BYTE*)0x0044884B, (DWORD)ReturnEarly4, 5);
		// createtrafficonteleport?
		Hooking::MakeJMP((BYTE*)0x00412803, (DWORD)ReturnEarly, 5);
		// jump over ai vehicle creation
		Hooking::MakeJMP((BYTE*)0x00412C1A, (DWORD)AIVehicleManagerHook, 5);
		// jump over cop vehicle creation
		Hooking::MakeJMP((BYTE*)0x0040FC04, (DWORD)CreateCopVehicleHook, 5);
		// jump over scripted ped creation
		Hooking::MakeJMP((BYTE*)0x004488D0, (DWORD)CreatePedsHook, 5);

		// Game Time Hooks
		//
		// Force 1.0 time multiplier always
		Hooking::MakeJMP((BYTE*)0x0045ace8, (DWORD)SetGameSpeedMultiplierHook, 6);

		auto clientController = ClientController::GetInstance();
		clientController.Connect();
	}
}