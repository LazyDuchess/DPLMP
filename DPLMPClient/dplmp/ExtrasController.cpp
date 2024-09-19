#include "ExtrasController.h"
#include "CarController.h"
#include "../dpl/CLifeSystem.h"
#include "../dpl/CCharacter.h"

NetworkedCar* ExtrasController::GetCarToEnterPassenger() {
	CarController* carController = CarController::GetInstance();
	CCharacter* playerCharacter = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	if (playerCharacter->GetVehicle() != nullptr) return nullptr;
	mat<float, 4, 4>* playerMatrix = playerCharacter->GetMatrix();
	vec<float,3> playerPos = GetPosition(playerMatrix);
	for (auto it = carController->Cars.begin(); it != carController->Cars.end(); it++) {
		NetworkedCar* car = it->second;
		if (car->Vehicle == nullptr) continue;
		vec<float, 3> carPos = car->Vehicle->GetPhysicsObject()->GetPosition();
		vec<float, 3> diff = playerPos - carPos;
		float dist = mag_sqr(diff);
		if (dist < PassengerCarDistance) {
			return car;
		}
	}
	return nullptr;
}

void ExtrasController::OnPassengerButtonPressed() {
	NetworkedCar* passengerCar = GetCarToEnterPassenger();
	if (passengerCar == nullptr) return;
	CCharacter* playerCharacter = CLifeSystem::GetInstance()->Player->DriverBehaviour->GetCharacter();
	playerCharacter->EnterVehicle(passengerCar->Vehicle, 1, false);
}

void ExtrasController::Step() {
	if (((GetAsyncKeyState(0x47) & 0x8001) == 0x8001))
		OnPassengerButtonPressed();
}