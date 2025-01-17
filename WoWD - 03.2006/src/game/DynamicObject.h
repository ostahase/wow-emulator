// Copyright (C) 2004 WoW Daemon
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef WOWSERVER_DYNAMICOBJECT_H
#define WOWSERVER_DYNAMICOBJECT_H

#include "Object.h"

struct SpellEntry;

class DynamicObject : public Object
{
public:
    DynamicObject( );
	~DynamicObject( );

    void Create( uint32 guidlow, Unit * caster, SpellEntry * spell, float x, float y, float z, uint32 duration ,Spell *sp);
    void Update(uint32 p_time);
    void DealDamage();
    void PeriodicTriggerDamage(uint32 damage, uint32 tick, float radius);

protected:

    float _CalcDistance(float sX, float sY, float sZ, float dX, float dY, float dZ);
	Spell* m_castspell;
    Unit* m_caster;
    SpellEntry * m_spell;
    uint32 m_PeriodicDamage;
    uint32 m_PeriodicDamageTick;
    uint32 m_PeriodicDamageCurrentTick;
    float m_PeriodicDamageRadius;

    uint32 m_aliveDuration;
};

#endif

