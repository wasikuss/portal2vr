#pragma once

#include <math.h>
#include "game.h"
#include "offsets.h"

class Game;

class CNewParticleEffect
{
public:
	inline void SetControlPoint(int nWhichPoint, const Vector& v) {
		typedef int(__thiscall* tSetControlPoint)(void* thisptr, int nWhichPoint, const Vector& v);
		static tSetControlPoint oSetControlPoint = (tSetControlPoint)(g_Game->m_Offsets->SetControlPoint.address);

		oSetControlPoint(this, nWhichPoint, v);
	};
};
