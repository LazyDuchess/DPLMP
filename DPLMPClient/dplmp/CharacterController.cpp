#include "CharacterController.h"
#include "../../DPLMPCommon/PLMessageIdentifiers.h"
#include "BitStream.h"

CharacterController::CharacterController() {
	_charByIndex = std::map<unsigned int, NetworkedCharacter*>();
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
    }
}