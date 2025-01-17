#pragma once
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

//struct SpellEntry;
class Unit;

class DynamicObject : public Object
{
public:
    DynamicObject( );

    void Create( uint32 guidlow, Unit * caster, SpellEntry * spell, float x, float y, float z, uint32 duration );
    void Update (int32 p_time);
    void DealDamage();
    void PeriodicTriggerDamage(uint32 damage, uint32 tick, float radius)
    {
        m_PeriodicDamage = damage;
        m_PeriodicDamageTick = tick;
        m_PeriodicDamageCurrentTick = tick;
        m_PeriodicDamageRadius = radius;
    }

protected:
    Unit* m_caster;
    SpellEntry * m_spell;
    int32 m_PeriodicDamage;
    int32 m_PeriodicDamageTick;
    int32 m_PeriodicDamageCurrentTick;
    float m_PeriodicDamageRadius;

    int32 m_aliveDuration;
};

#endif

