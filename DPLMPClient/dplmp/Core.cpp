#include "../framework.h"
#include "Core.h"
#include "Hooking.h"
#include "ClientController.h"
#include "ExtrasController.h"
#include <iostream>
#include <vector>
#include <chrono>
#include "../../DPLMPCommon/CommonDefines.h"
#include "../dpl/CLifeEventDataManager.h"
#include "../dpl/CLoadingScreen.h"
#include "../dpl/SpoolableResourceManager.h"
#include "../dpl/CStateFrontend.h"
#include "../dpl/CStateReload.h"
#include "../dpl/CHandling.h"
#include "../dpl/CLifeSystem.h"
#include "../dpl/CCharacterManager.h"

mINI::INIStructure Core::Ini;
std::vector<EventListener*> eventListeners;
ClientController* Core::_clientController = new ClientController();
bool Core::InGame = false;

typedef std::chrono::steady_clock steady_clock;
float Core::FixedDeltaTime = 0.0;
float Core::DeltaTime = 0.0;
steady_clock::time_point fixedTimePoint;
steady_clock::time_point deltaTimePoint;
steady_clock::time_point beginLoadingTime;
const float minimumLoadingTime = 0.1;

const char* thenLevelsOverride = "DPLMP\\level_then.txt";
const char* nowLevelsOverride = "DPLMP\\level_now.txt";

// TODO: come up with something better, hardcoded preferably.
void OverrideLevels(const char* str) {
	Hooking::WriteToMemory(0x00467e27, &str, 4);
}

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
		mov dword ptr[esp + 04], 0x3f800000
		movss xmm0, [esp + 04]
		mov eax, 0x0045ACEE
		jmp eax
	}
}

bool enteringGame = false;

void __stdcall OnGameStepHook() {
	steady_clock::time_point now = steady_clock::now();
	std::chrono::duration<float> fixeddelta = now - fixedTimePoint;
	std::chrono::duration<float> delta = now - deltaTimePoint;
	Core::FixedDeltaTime = fixeddelta.count();
	Core::DeltaTime = delta.count();
	for (auto listener : eventListeners) {
		listener->FrameStep();
	}
	if (Core::FixedDeltaTime >= TICKRATE) {
		for (auto listener : eventListeners) {
			listener->Step();
		}
		fixedTimePoint = steady_clock::now();
	}

	// HACK? Loading screen will wait for life events that are not stepping, so instead we just disable it when resources are loaded.
	if (Core::InGame && std::chrono::duration<float>(now - beginLoadingTime).count() > minimumLoadingTime) {
		SpoolableResourceManager* resManager = SpoolableResourceManager::GetInstance();
		CLoadingScreen* loadingScreen = CLoadingScreen::GetInstance();
		if (loadingScreen != nullptr && resManager != nullptr) {
			int spoolStatus = resManager->GetStatus();
			if (spoolStatus != 0)
				loadingScreen->Deactivate();
		}
	}
	/*
	if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
	{
		mat<float,4,4>* charMatrix = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetMatrix();
		vec<float, 4> charPos = { charMatrix->a[3][0] + 5.0, charMatrix->a[3][1] , charMatrix->a[3][2], 1 };
		CCharacterManager::GetInstance()->CreateCharacter(29, &charPos, 1);
	}*/
	if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
	{
		CCharacter* playerChar = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
		CVehicle* car = CarController::GetInstance()->Cars.begin()->second->Vehicle;
		//playerChar->EnterVehicle(car, 0, false);
		playerChar->EnterVehicleImmediate(car, 1, true);
	}

	deltaTimePoint = steady_clock::now();
}

bool __stdcall ShouldSendManipulationPacket(CHandling* handling) {
	CVehicle* vehicle = handling->GetVehicle();
	if (vehicle == nullptr) return true;
	CCharacter* playerChar = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	if (playerChar->GetCarSeat() != 0) return false;
	CarController* carController = CarController::GetInstance();
	if (carController == nullptr) return true;
	NetworkedCar* netCar = carController->GetCarForVehicle(vehicle);
	if (netCar == nullptr) return true;
	return netCar->ShouldBeNetworkedByLocalPlayer();
}

void __declspec(naked) CarHandlingSendManipulationPacketHook() {
	__asm {
		push edi
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		push esi
		call ShouldSendManipulationPacket
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop edi
		cmp al, 00
		je TrueCond
		//Original
		cmp byte ptr [esi+0x18C],00
		mov eax, 0x0058CB7A
		jmp eax
		TrueCond:
			mov eax, 0x0058CB6D
			jmp eax
	}
}

void __stdcall OnPlayerCreatedHook() {
	for (auto listener : eventListeners) {
		listener->OnPlayerCreated();
	}
}

void __stdcall OnExitInGameStateHook() {
	Core::InGame = false;
	for (auto listener : eventListeners) {
		listener->OnExitInGameState();
	}
}

void __stdcall OnEnterInGameStateHook() {
	Core::InGame = true;
	CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
	for (auto listener : eventListeners) {
		listener->OnEnterInGameState();
	}
}

void __stdcall OnLoadingScreenActivateHook() {
	beginLoadingTime = steady_clock::now();
}

void __declspec(naked) LoadingScreenActivateHook() {
	__asm {
		push edi
		push eax
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		call OnLoadingScreenActivateHook
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop eax
		pop edi
		//Original
		push esi
		mov esi, ecx
		lea eax, [esi + 0x3d9]
		mov edi, 0x004a76f8
		jmp edi
	}
}

void __declspec(naked) EnterInGameStateHook() {
	__asm {
		push edi
		push eax
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		call OnEnterInGameStateHook
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop eax
		pop edi
		//Original
		pop esi
		mov esp, ebp
		pop ebp
		ret
	}
}

void __declspec(naked) ExitInGameStateHook() {
	__asm {
		push edi
		push eax
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		call OnExitInGameStateHook
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop eax
		pop edi
		//Original
		mov eax, 0x00548250
		call eax
		mov eax, 0x00472401
		jmp eax
	}
}

void __declspec(naked) GameStepHook() {
	__asm {
		push edi
		push eax
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		call OnGameStepHook
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop eax
		pop edi
		// go back home
		mov eax, 0x005715EE
		jmp eax
	}
}

void __declspec(naked) CreatePlayerHook() {
	__asm {
		mov eax, 0x0047F841
		call eax //CreatePlayer
		push edi
		push eax
		push edx
		push ecx
		push ebp
		push ebx
		push esi
		call OnPlayerCreatedHook
		pop esi
		pop ebx
		pop ebp
		pop ecx
		pop edx
		pop eax
		pop edi
		mov eax, 0x0047F8F9
		jmp eax
	}
}

ClientController* Core::GetClientController() {
	return Core::_clientController;
}

void Core::Initialize() {
	mINI::INIFile file("DPLMP.ini");
	file.read(Ini);

	_clientController->ServerHost = Ini["Server"]["Host"];
	_clientController->ServerPort = std::stoi(Ini["Server"]["Port"]);

	if (Ini["General"]["Console"] == "true") {
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
	static char jmpByte = 0xEB;


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
	// jump over constantly trying to create uninitialised vehicles. NVM this patch caused lag for some reason. Still testing.
	// Hooking::MakeJMP((BYTE*)0x00431EB3, 0x00432001, 6);
	// same as above, but without the lag!
	Hooking::Nop((BYTE*)0x0042d007, 2);
	Hooking::MakeJMP((BYTE*)0x0042D010, 0x0042EA8D, 6);
	// end patch

	// Game Time Hooks
	//
	// Force 1.0 time multiplier always
	Hooking::MakeJMP((BYTE*)0x0045ace8, (DWORD)SetGameSpeedMultiplierHook, 6);

	// Events
	//
	// CoreCreate Character - CreatePlayer case.
	Hooking::MakeJMP((BYTE*)0x0047f8ed, (DWORD)CreatePlayerHook, 7);
	// GameStep? - Main game loop.
	//Hooking::MakeJMP((BYTE*)0x00545aa3, (DWORD)GameStepHook, 6);
	Hooking::MakeJMP((BYTE*)0x005715E8, (DWORD)GameStepHook, 6);
	// DestroyInGameStuff? Exiting ingame state
	Hooking::MakeJMP((BYTE*)0x004723fc, (DWORD)ExitInGameStateHook, 5);
	// CLifeSystem::Initialise Entering ingame state
	Hooking::MakeJMP((BYTE*)0x004720bf, (DWORD)EnterInGameStateHook, 5);

	// Disable autosave
	Hooking::WriteToMemory(0x004a97d5, &jmpByte, 1);

	// Disable CLifeEventDataManager::Step(), which stops life events from progressing.
	// Good enough for now, but ideally we'd prevent life events from being created/end them all and delete mission markers while also not crashing.
	Hooking::Nop((BYTE*)0x004721b3, 5);

	// Hit us up on CLoadingScreen::Activate
	Hooking::MakeJMP((BYTE*)0x004a76ef, (DWORD)LoadingScreenActivateHook, 9);

	// Keep game running while unfocused. Injects main game loop.
	Hooking::Nop((BYTE*)0x005715E2, 2);
	//Hooking::Nop((BYTE*)0x005715E8, 6);

	// Only send controls for local cars. Inject CCarHandling::SendManipulationPacket.
	Hooking::MakeJMP((BYTE*)0x0058cb73, (DWORD)CarHandlingSendManipulationPacketHook, 7);

	// Spawn us immediately
	OverrideLevels(thenLevelsOverride);

	RegisterEventListener(_clientController);
	RegisterEventListener(new ExtrasController());
}

void Core::RegisterEventListener(EventListener* listener) {
	eventListeners.push_back(listener);
}

void Core::AllowMissionObjectCreation(bool allow) {
	static char allowByte[] = { 0x74 };
	static char disallowByte[] = { 0xEB };
	if (allow) {
		Hooking::WriteToMemory(0x0046ba46, allowByte, 1);
	}
	else
	{
		Hooking::WriteToMemory(0x0046ba46, disallowByte, 1);
	}
}