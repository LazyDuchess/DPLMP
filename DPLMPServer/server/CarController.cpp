#include "CarController.h"
#include "PLMessageIdentifiers.h"
#include "ServerController.h"
#include "OwnershipKinds.h"
#include "Core.h"

void CarController::SpawnTestCars() {
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Andec_Punk, { -2192.623047, 0.983009, 3798.156982 }, { -0.004596, 0.953209, 0.003860, 0.302252 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Colonna_Racer, { -2206.804199, 0.971373, 3799.105713 }, { -0.006162, 0.999548, 0.002203, 0.029355 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Colonna_Racer, { -2210.195312, 0.652893, 3797.874023 }, { 0.000468, 0.927555, 0.005415, 0.373647 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Cerrano_Punk, { -2222.952881, 0.944798, 3777.477783 }, { 0.000233, 0.999407, 0.001934, -0.034380 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Hotrod, { -2256.548340, 0.963347, 3786.693848 }, { -0.001177, 0.841995, 0.002749, 0.539477 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_M700, { -2230.223877, 0.892499, 3833.030762 }, { -0.002102, -0.024875, -0.000051, 0.999688 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Ram_Raider, { -2149.957520, 0.967345, 3825.753174 }, { -0.004953, 0.679953, 0.003359, 0.733231 }, { 1,1,1 });
    CreateVehicle(tVehicleModelUID::tVehicleModelUID_Melizzano_Racer, { -2154.103760, 0.962602, 3770.465332 }, { -0.001713, 0.980018, 0.001385, -0.198899 }, { 1,1,1 });
}

NetworkedCar* CarController::CreateVehicle(tVehicleModelUID model, vec<float, 3> position, quat<float> rotation, vec<float, 3> color) {
    NetworkedCar* car = new NetworkedCar(model, position, rotation, &_carUIDProvider, color);
    _cars[car->UID] = car;
    return car;
}

void CarController::Initialize() {
    SpawnTestCars();
    _ownershipRefreshTimer = 0;
}

void CarController::RefreshOwnerships() {
    for (auto const& car : _cars)
    {
        UpdateCarOwnership(car.second);
        BroadcastOwnership(car.second);
    }
}

void CarController::Step() {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_UPDATE);
    bs.Write((unsigned int)_cars.size());
    for (auto const& car : _cars)
    {
        bs.Write(car.second->UID);
        car.second->WriteUpdate(&bs);
    }
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::UNRELIABLE, 0);
    _ownershipRefreshTimer += Core::FixedDeltaTime;
    if (_ownershipRefreshTimer >= OwnershipRefreshInterval) {
        RefreshOwnerships();
		_ownershipRefreshTimer = 0;
	}
}

void CarController::HandleClientConnected(Client* client) {

    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_FULLSTATE);
    bs.Write((unsigned int)_cars.size());
    for (auto const& car : _cars)
    {
        bs.Write(car.second->UID);
        car.second->WriteFullState(&bs);
    }
    ServerController::GetInstance().Send(&bs, client->GUID, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

void CarController::HandleClientDisconnected(Client* client) {

    for (auto const& car : _cars)
    {
        if (car.second->Owner == client->GUID) {
            car.second->Owner = RakNet::UNASSIGNED_RAKNET_GUID;
            car.second->OwnershipKind = OwnershipKinds::Normal;
            BroadcastOwnership(car.second);
        }
    }
}

void CarController::BroadcastOwnership(NetworkedCar* car) {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_OWNERSHIP);
    bs.Write(car->UID);
    bs.Write(car->Owner);
    bs.Write(car->OwnershipKind);
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::LOW_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

void CarController::HandlePacket(RakNet::Packet* packet) {
    RakNet::BitStream bs(packet->data, packet->length, false);
    bs.IgnoreBytes(1);
    switch (packet->data[0]) {

        case ID_CARCONTROLLER_MAKEMEOWNER:
        {
            unsigned int uid;
            bs.Read(uid);
            if (_cars.find(uid) != _cars.end()) {
                _cars[uid]->Owner = packet->guid;
                _cars[uid]->OwnershipKind = OwnershipKinds::Driving;
                BroadcastOwnership(_cars[uid]);
            }
        }
            break;

        case ID_CARCONTROLLER_RELEASEOWNERSHIP:
        {
            unsigned int uid;
            RakNet::RakNetGUID owner;
            bs.Read(uid);
            if (_cars.find(uid) != _cars.end()) {
                if (_cars[uid]->Owner == packet->guid) {
                    // TODO: Find situable owner on release and disconnect.
                    _cars[uid]->OwnershipKind = OwnershipKinds::Normal;
                    BroadcastOwnership(_cars[uid]);
                }
            }
        }
            break;

        case ID_CARCONTROLLER_UPDATE:
        {
            unsigned int uid;
            bs.Read(uid);
            if (_cars.find(uid) != _cars.end()) {
                if (_cars[uid]->Owner == packet->guid) {
                    _cars[uid]->ReadUpdate(&bs);
                }
            }
        }
    }
}

void CarController::UpdateCarOwnership(NetworkedCar* car) {
    ServerController server = ServerController::GetInstance();
    if (server.Clients.size() == 0) {
		car->Owner = RakNet::UNASSIGNED_RAKNET_GUID;
		car->OwnershipKind = OwnershipKinds::Normal;
	}
    else
    {
        if (car->OwnershipKind == OwnershipKinds::Normal) {
            Client* closestClient = GetClientClosestToCar(car);
            if (closestClient == nullptr)
                car->Owner = RakNet::UNASSIGNED_RAKNET_GUID;
            else
                car->Owner = closestClient->GUID;
        }
    }
}

Client* CarController::GetClientClosestToCar(NetworkedCar* car) {
    Client* result = nullptr;
    float closestDistance = 9999999;
    for (auto const& client : ServerController::GetInstance().Clients)
    {
        if (client.second->Character == nullptr) continue;
        vec<float, 3> diff = client.second->Character->Position - car->Position;
        float dist = mag_sqr(diff);
        if (dist < closestDistance) {
            result = client.second;
            closestDistance = dist;
        }
    }
    return result;
}