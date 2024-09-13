#include "CarController.h"
#include "PLMessageIdentifiers.h"
#include "ServerController.h"

void CarController::Initialize() {

}

void CarController::Step() {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_UPDATE);
    bs.Write((unsigned int)_cars.size());
    for (auto const& car : _cars)
    {
        car.second->WriteUpdate(&bs);
    }
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::MEDIUM_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void CarController::HandleClientConnected(Client* client) {

    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_FULLSTATE);
    bs.Write((unsigned int)_cars.size());
    for (auto const& car : _cars)
    {
        car.second->WriteFullState(&bs);
    }
    ServerController::GetInstance().Send(&bs, client->GUID, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

void CarController::HandleClientDisconnected(Client* client) {

    for (auto const& car : _cars)
    {
        if (car.second->Owner == client->GUID) {
            car.second->Owner = RakNet::UNASSIGNED_RAKNET_GUID;
            BroadcastOwnership(car.second);
        }
    }
}

void CarController::BroadcastOwnership(NetworkedCar* car) {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CARCONTROLLER_OWNERSHIP);
    bs.Write(car->UID);
    bs.Write(car->Owner);
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

void CarController::HandlePacket(RakNet::Packet* packet) {
    RakNet::BitStream bs(packet->data, packet->length, false);
    bs.IgnoreBytes(1);
    switch (packet->data[0]) {
    case ID_CARCONTROLLER_MAKEMEOWNER:
    {
        unsigned int uid;
        RakNet::RakNetGUID owner;
        bs.Read(uid);
        bs.Read(owner);
        if (_cars.find(uid) != _cars.end()) {
            _cars[uid]->Owner = owner;
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
                    // TODO: Find situable owner.
                    _cars[uid]->Owner = RakNet::UNASSIGNED_RAKNET_GUID;
                    BroadcastOwnership(_cars[uid]);
                }
            }
        }
            break;
    }
}