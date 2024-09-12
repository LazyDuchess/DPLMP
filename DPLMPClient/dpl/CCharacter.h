#pragma once
#include <d3dx9math.h>

class CCharacter {
public:
	void SetModel(int model);
	void SetAnimType(int type);
	void SwapSkin(int animations, int skin);
	D3DXMATRIX* GetMatrix();
};