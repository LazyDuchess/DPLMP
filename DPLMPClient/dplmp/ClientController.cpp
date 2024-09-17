#include "../framework.h"
#include "ClientController.h"
#include "RakPeerInterface.h"
#include "RakSleep.h"
#include "Core.h"
#include <stdio.h>
#include <mutex>
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "../dpl/CLifeSystem.h"
#include "../dpl/SpoolableResourceManager.h"
#include "../dpl/CLifeEventDataManager.h"
#include "../dpl/CLifeInstances.h"
#include "../../DPLMPCommon/PLMath.h"

ClientController::ClientController() {
	_timeController = new TimeController();
	_carController = new CarController();
	Client = nullptr;
	_serverAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	MyGUID = RakNet::UNASSIGNED_RAKNET_GUID;
	ServerPort = 0;
	ServerHost = "127.0.0.1";
}

void ClientController::Connect() {
	_serverAddress.SetBinaryAddress(ServerHost.c_str());
	_serverAddress.SetPortHostOrder(ServerPort);
	Client = RakNet::RakPeerInterface::GetInstance();
	Client->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SocketDescriptor socketDescriptor(0, 0);
	socketDescriptor.socketFamily = AF_INET;
	RakNet::StartupResult sr;
	sr = Client->Startup(4, &socketDescriptor, 1);
	if (sr != RakNet::RAKNET_STARTED)
	{
		printf("Client failed to start. Error=%i\n", sr);
	}

	printf("Started client on %s\n", Client->GetMyBoundAddress().ToString(true));

	RakNet::ConnectionAttemptResult result = Client->Connect(ServerHost.c_str(), ServerPort, 0, 0);
	while (GetConnectionState() == RakNet::ConnectionState::IS_CONNECTING || GetConnectionState() == RakNet::ConnectionState::IS_PENDING)
	{
		RakSleep(100);
	}
	MyGUID = Client->GetMyGUID();
	printf("Our GUID is %s\n", MyGUID.ToString());
	printf("Connection state: %i\n", GetConnectionState());
}

void ClientController::Disconnect() {
	_carController->OnDisconnect();
	if (Client == nullptr)
		return;
	Client->CloseConnection(_serverAddress, true);
}

RakNet::ConnectionState ClientController::GetConnectionState() {
	if (Client == nullptr)
		return RakNet::IS_NOT_CONNECTED;
	return Client->GetConnectionState(_serverAddress);
}

void ClientController::HandlePackets() {
	if (Client != nullptr) {
		RakNet::Packet* packet;
		for (packet = Client->Receive(); packet; Client->DeallocatePacket(packet), packet = Client->Receive())
		{
			switch (packet->data[0]) {
			case ID_TIMEOFDAY:
				_timeController->HandlePacket(packet);
				break;
			case ID_CARCONTROLLER_FULLSTATE:
			case ID_CARCONTROLLER_OWNERSHIP:
			case ID_CARCONTROLLER_UPDATE:
				_carController->HandlePacket(packet);
				break;
			}
		}
	}
}

void PrintMatrix(mat<float, 4, 4>* matrix) {
	printf("[ %f %f %f %f ]\n", matrix->a[0][0], matrix->a[0][1], matrix->a[0][2], matrix->a[0][3]);
	printf("[ %f %f %f %f ]\n", matrix->a[1][0], matrix->a[1][1], matrix->a[1][2], matrix->a[1][3]);
	printf("[ %f %f %f %f ]\n", matrix->a[2][0], matrix->a[2][1], matrix->a[2][2], matrix->a[2][3]);
	printf("[ %f %f %f %f ]\n", matrix->a[3][0], matrix->a[3][1], matrix->a[3][2], matrix->a[3][3]);
}

void ClientController::FrameStep() {
	_carController->FrameStep();
}

void ClientController::Step() {
    HandlePackets();
	_carController->Step();
    _timeController->Step();
    // EXPERIMENTS
    // Crashes when going near a side mission area. Might work if the save has no side missions available?
    /*
    if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
    {
        CLifeEventDataManager::GetInstance()->EndAllLifeEvents();
    }*/
	if (((GetAsyncKeyState(VK_NUMPAD0) & 0x8001) == 0x8001))
	{
		CHandling* carHandling = nullptr;
		CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle()->GetHandling(&carHandling);
		PHBaseObj* carPhysics = carHandling->GetPhysicsObject();
		auto physPos = carPhysics->GetPosition();
		auto physRot = carPhysics->GetRotation();
		printf("Physics Position: %f %f %f\n", physPos.a[0], physPos.a[1], physPos.a[2]);
		printf("Physics Rotation: %f %f %f %f\n", physRot.a[0], physRot.a[1], physRot.a[2], physRot.a[3]);
		printf("Visual Matrix:\n");
		PrintMatrix(carPhysics->GetMatrix());
		printf("MatrixFromPhysicsTransform:\n");
		mat<float, 4, 4> fromPhysicsTransform = MatrixFromPhysicsTransform(physPos, physRot);
		PrintMatrix(&fromPhysicsTransform);
		printf("This car's handling is at %p\n", carHandling);
	}
	/*
    if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
    {
		CHandling* carHandling = nullptr;
		CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle()->GetHandling(&carHandling);
		PHBaseObj* carPhysics = carHandling->GetPhysicsObject();
        //printf("Our physics object addr is %p\n", carPhysics);
		auto pos = carPhysics->GetPosition();
		auto rot = carPhysics->GetRotation();
		printf("Our position is %f %f %f\n", pos.a[0], pos.a[1], pos.a[2]);
		printf("Our rotation is %f %f %f %f\n", rot.a[0], rot.a[1], rot.a[2], rot.a[3]);
        //PrintMatrix(CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle()->GetMatrix());
    }

	if (((GetAsyncKeyState(VK_NUMPAD2) & 0x8001) == 0x8001))
	{
		CHandling* carHandling = nullptr;
		CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle()->GetHandling(&carHandling);
		PHBaseObj* carPhysics = carHandling->GetPhysicsObject();
		carPhysics->SetPosition({ -2154.103760, 0.962602, 3770.465332 });
		carPhysics->SetRotation({ -0.001713, 0.980018, 0.001385, -0.198899 });
	}*/

	/*
	if (((GetAsyncKeyState(VK_NUMPAD1) & 0x8001) == 0x8001))
	{
		CVehicle* veh = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle();
		PHBaseObj* carPhysics = veh->GetPhysicsObject();
		printf("Object to world:\n");
		PrintMatrix(carPhysics->GetObjectToWorldMatrix());
		printf("Visual matrix:\n");
		PrintMatrix(carPhysics->GetMatrix());

		quat<float> physicsRotation = carPhysics->GetRotation();
		vec<float, 3> physicsPosition = carPhysics->GetPosition();
		mat<float, 4, 4> physMatrix;
		SetPosition(&physMatrix, physicsPosition);
		SetQuaternionRotation(&physMatrix, physicsRotation);

		printf("Physics matrix:\n");
		PrintMatrix(&physMatrix);
		printf("Physics Rotation: %f %f %f %f\n", physicsRotation.a[0], physicsRotation.a[1], physicsRotation.a[2], physicsRotation.a[3]);
	}
	*/
	/*
	if (((GetAsyncKeyState(VK_NUMPAD2) & 0x8001) == 0x8001))
	{
		CVehicle* veh = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetVehicle();
		PHBaseObj* carPhysics = veh->GetPhysicsObject();

		quat<float> physicsRotation = carPhysics->GetRotation();
		vec<float, 3> physicsPosition = carPhysics->GetPosition();
		mat<float,4,4>* visualMatrix = carPhysics->GetMatrix();

		printf("Physics Orientation:\n");
		printf("%f %f %f %f\n", physicsRotation.a[0], physicsRotation.a[1], physicsRotation.a[2], physicsRotation.a[3]);
		quat<float> visualOrientation = GetQuaternionRotation(visualMatrix);

		printf("Visual Orientation:\n");
		printf("%f %f %f %f\n", visualOrientation.a[0], visualOrientation.a[1], visualOrientation.a[2], visualOrientation.a[3]);

		mat<float, 4, 4> physMatrix;
		SetPosition(&physMatrix, physicsPosition);
		SetQuaternionRotation(&physMatrix, physicsRotation);

		printf("Physics Matrix:\n");
		PrintMatrix(&physMatrix);

		printf("Visual Matrix:\n");
		PrintMatrix(visualMatrix);
	}*/
}

void ClientController::Send(const RakNet::BitStream* bitStream, PacketPriority priority, PacketReliability reliability, char orderingChannel) {
	Client->Send(bitStream, priority, reliability, orderingChannel, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ClientController::OnPlayerCreated() {
	printf("Player Created!\n");
}

void ClientController::OnEnterInGameState() {
    printf("Now In-Game!\n");
    SpoolableResourceManager::GetInstance()->SetEntityPriority(SpooledPackageType::Characters, 123, SpoolPriority::Request);
    CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->SwapSkin(123, 123);
	mat<float,4,4>* playerMatrix = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter()->GetMatrix();
	// spawn at jersey
	SetPosition(playerMatrix, { -2189.333496, 0.067905, 3787.999512 });
    Connect();
}

void ClientController::OnExitInGameState() {
	printf("No longer In-Game!\n");
	Disconnect();
}