#include "CharacterController.h"
#include "ServerController.h"
#include <PLMessageIdentifiers.h>

NetworkedCharacter* CharacterController::CreatePlayerCharacter(Client* client, vec<float, 3> position, quat<float> rotation, int model) {
    NetworkedCharacter* ch = new NetworkedCharacter(model, position, rotation, &_charUIDProvider);
    ch->Owner = client->GUID;
    ch->Player = true;
    _characters[ch->UID] = ch;
    return ch;
}

void CharacterController::HandleClientConnected(Client* client) {
    CreatePlayerCharacter(client, { -2189.333496, 0.067905, 3787.999512 }, {0,0,1,0}, 123);
    BroadcastCharacters();
}

void CharacterController::BroadcastCharacters() {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CHARACTERCONTROLLER_FULLSTATE);
    bs.Write((unsigned int)_characters.size());
    for (auto const& ch : _characters)
    {
        bs.Write(ch.second->UID);
        ch.second->WriteFullState(&bs);
    }
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
}

void CharacterController::Step() {
    RakNet::BitStream bs;
    bs.Write((unsigned char)ID_CHARACTERCONTROLLER_UPDATE);
    bs.Write((unsigned int)_characters.size());
    for (auto const& character : _characters)
    {
        bs.Write(character.second->UID);
        character.second->WriteUpdate(&bs);
    }
    ServerController::GetInstance().Broadcast(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::UNRELIABLE, 0);
}

void CharacterController::HandlePacket(RakNet::Packet* packet) {
    RakNet::BitStream bs(packet->data, packet->length, false);
    bs.IgnoreBytes(1);
    switch (packet->data[0]) {
    case ID_CHARACTERCONTROLLER_UPDATE:
    {
        unsigned int uid;
        bs.Read(uid);
        if (_characters.find(uid) != _characters.end()) {
            if (_characters[uid]->Owner == packet->guid) {
                _characters[uid]->ReadUpdate(&bs);
            }
        }
    }
    case ID_CHARACTERCONTROLLER_ENTERVEHICLE:
    {
        RakNet::BitStream outStream(packet->data, packet->length, true);
        ServerController::GetInstance().Broadcast(&outStream, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0);
    }
    }
}