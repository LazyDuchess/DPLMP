#include "CharacterController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "BitStream.h"
#include "Core.h"

CharacterController* CharacterController::_instance = nullptr;

CharacterController::CharacterController() {
	_charByIndex = std::map<unsigned int, NetworkedCharacter*>();
    _instance = this;
}

CharacterController* CharacterController::GetInstance() {
    return _instance;
}

void CharacterController::Step() {
    for (auto const& ch : _charByIndex)
    {
        ch.second->Step();
    }
}

void CharacterController::FrameStep() {
    for (auto const& ch : _charByIndex)
    {
        ch.second->FrameStep();
    }
}

void CharacterController::OnDisconnect() {
    _charByIndex.clear();
}

void CharacterController::SendEnterVehiclePacket(unsigned int uid, unsigned int vehicleId, int seat, bool toDriverSeat) {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CHARACTERCONTROLLER_ENTERVEHICLE);
    bs.Write(uid);
    bs.Write(vehicleId);
    bs.Write(seat);
    bs.Write(toDriverSeat);
    Core::GetClientController()->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

NetworkedCharacter* CharacterController::GetLocalCharacter() {
    for (auto it = _charByIndex.begin(); it != _charByIndex.end(); it++) {
        NetworkedCharacter* ch = it->second;
        if (ch->Player && ch->Owner == Core::GetClientController()->MyGUID)
            return ch;
    }
    return nullptr;
}

NetworkedCharacter* CharacterController::GetNetCharacterForCharacter(CCharacter* character) {
    for (auto it = _charByIndex.begin(); it != _charByIndex.end(); it++) {
        NetworkedCharacter* ch = it->second;
        if (ch->Character == character)
            return ch;
    }
    return nullptr;
}

void CharacterController::HandlePacket(RakNet::Packet* packet) {
    RakNet::BitStream bs(packet->data, packet->length, false);
    bs.IgnoreBytes(1);
    switch (packet->data[0]) {
    case ID_CHARACTERCONTROLLER_FULLSTATE:
    {
        int charCount = 0;
        bs.Read(charCount);
        for (int i = 0; i < charCount; i++) {
            unsigned int uid = 0;
            bs.Read(uid);
            if (_charByIndex.find(uid) == _charByIndex.end()) {
                NetworkedCharacter* ch = new NetworkedCharacter();
                ch->UID = uid;
                ch->ReadFullState(&bs);
                _charByIndex[ch->UID] = ch;
                ch->Spawn();
            }
            else
            {
                _charByIndex[uid]->ReadFullState(&bs);
            }
        }
        break;
    }
    case ID_CHARACTERCONTROLLER_UPDATE:
    {
        int charCount = 0;
        bs.Read(charCount);
        for (int i = 0; i < charCount; i++) {
            unsigned int uid = 0;
            bs.Read(uid);
            if (_charByIndex.find(uid) != _charByIndex.end()) {
                _charByIndex[uid]->ReadUpdate(&bs);
            }
        }
        break;
    }
    case ID_CHARACTERCONTROLLER_ENTERVEHICLE:
    {
        unsigned int uid = 0;
        bs.Read(uid);
        unsigned int vehicleId = 0;
        bs.Read(vehicleId);
        int seat = 0;
        bs.Read(seat);
        bool toDriverSeat = false;
        bs.Read(toDriverSeat);
        if (_charByIndex.find(uid) != _charByIndex.end()) {
            if (_charByIndex[uid]->Owner == Core::GetClientController()->MyGUID) return;
            CCharacter* ch = _charByIndex[uid]->Character;
            NetworkedCar* netCar = nullptr;
            CarController* carController = CarController::GetInstance();
            if (carController->Cars.find(vehicleId) != carController->Cars.end() && ch != nullptr) {
                netCar = carController->Cars[vehicleId];
                if (netCar->Vehicle != nullptr)
                    ch->EnterVehicle(netCar->Vehicle, seat, toDriverSeat);
            }
        }
        break;
    }
    }
}