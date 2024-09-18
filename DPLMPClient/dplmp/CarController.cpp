#include "CarController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"

CarController* CarController::_instance = nullptr;

CarController::CarController() {
	_carByIndex = std::map<unsigned int, NetworkedCar*>();
    _instance = this;
}

CarController* CarController::GetInstance() {
    return _instance;
}

NetworkedCar* CarController::GetCarForVehicle(CVehicle* vehicle) {
    if (vehicle == nullptr) return nullptr;
	for (auto const& car : _carByIndex)
	{
		if (car.second->Vehicle == vehicle) {
			return car.second;
		}
	}
	return nullptr;
}

void CarController::Step() {
    for (auto const& car : _carByIndex)
    {
        car.second->Step();
    }
}

void CarController::FrameStep() {
    for (auto const& car : _carByIndex)
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
            _carByIndex[car->UID] = car;
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
            if (_carByIndex.find(uid) != _carByIndex.end()) {
                _carByIndex[uid]->ReadUpdate(&bs);
			}
        }
        break;
    }
    case ID_CARCONTROLLER_OWNERSHIP:
    {
        unsigned int uid = 0;
        bs.Read(uid);
        if (_carByIndex.find(uid) != _carByIndex.end()) {
            bs.Read(_carByIndex[uid]->Owner);
            bs.Read(_carByIndex[uid]->OwnershipKind);
        }
        break;
    }
    }
}

void CarController::UpdateAllTransforms() {
    for (auto const& car : _carByIndex)
    {
        car.second->UpdateTransforms(true);
    }
}

void CarController::OnDisconnect() {
    _carByIndex.clear();
}