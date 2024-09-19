#include "CarController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"

CarController* CarController::_instance = nullptr;

CarController::CarController() {
	Cars = std::map<unsigned int, NetworkedCar*>();
    _instance = this;
}

CarController* CarController::GetInstance() {
    return _instance;
}

NetworkedCar* CarController::GetCarForVehicle(CVehicle* vehicle) {
    if (vehicle == nullptr) return nullptr;
	for (auto const& car : Cars)
	{
		if (car.second->Vehicle == vehicle) {
			return car.second;
		}
	}
	return nullptr;
}

void CarController::Step() {
    for (auto const& car : Cars)
    {
        car.second->Step();
    }
}

void CarController::FrameStep() {
    for (auto const& car : Cars)
    {
        car.second->FrameStep();
    }
}

void CarController::HandlePacket(RakNet::Packet* packet) {
    RakNet::BitStream bs(packet->data, packet->length, false);
    bs.IgnoreBytes(1);
    switch (packet->data[0]) {
    case ID_CARCONTROLLER_FULLSTATE:
    {
        int carCount = 0;
        bs.Read(carCount);
        for (int i = 0; i < carCount; i++) {
            NetworkedCar* car = new NetworkedCar();
            bs.Read(car->UID);
            car->ReadFullState(&bs);
            car->RequestSpawnCar();
            Cars[car->UID] = car;
        }
        break;
    }
    case ID_CARCONTROLLER_UPDATE:
    {
        int carCount = 0;
        bs.Read(carCount);
        for (int i = 0; i < carCount; i++) {
            unsigned int uid = 0;
            bs.Read(uid);
            if (Cars.find(uid) != Cars.end()) {
                Cars[uid]->ReadUpdate(&bs);
			}
        }
        break;
    }
    case ID_CARCONTROLLER_OWNERSHIP:
    {
        unsigned int uid = 0;
        bs.Read(uid);
        if (Cars.find(uid) != Cars.end()) {
            bs.Read(Cars[uid]->Owner);
            bs.Read(Cars[uid]->OwnershipKind);
        }
        break;
    }
    }
}

void CarController::UpdateAllTransforms() {
    for (auto const& car : Cars)
    {
        car.second->UpdateTransforms(true);
    }
}

void CarController::OnDisconnect() {
    Cars.clear();
}