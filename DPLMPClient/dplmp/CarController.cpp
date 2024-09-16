#include "CarController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"

CarController* CarController::_instance = nullptr;

CarController::CarController() {
	_cars = std::map<unsigned int, NetworkedCar*>();
    _instance = this;
}

CarController* CarController::GetInstance() {
    return _instance;
}

void CarController::Step() {
    for (auto const& car : _cars)
    {
        car.second->Step();
    }
}

void CarController::FrameStep() {
    for (auto const& car : _cars)
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
            _cars[car->UID] = car;
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
            if (_cars.find(uid) != _cars.end()) {
                _cars[uid]->ReadUpdate(&bs);
			}
        }
        break;
    }
    case ID_CARCONTROLLER_OWNERSHIP:
    {
        unsigned int uid = 0;
        bs.Read(uid);
        if (_cars.find(uid) != _cars.end()) {
            bs.Read(_cars[uid]->Owner);
            bs.Read(_cars[uid]->OwnershipKind);
        }
        break;
    }
    }
}

void CarController::UpdateAllTransforms() {
    for (auto const& car : _cars)
    {
        car.second->UpdateTransforms();
    }
}

void CarController::OnDisconnect() {
    _cars.clear();
}