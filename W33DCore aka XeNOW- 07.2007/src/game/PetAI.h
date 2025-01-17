/* 
 * Copyright (C) 2005,2006,2007 MaNGOS <http://www.mangosproject.org/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOS_PETAI_H
#define MANGOS_PETAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;

class MANGOS_DLL_DECL PetAI : public CreatureAI
{
    public:

        PetAI(Creature &c);

        void MoveInLineOfSight(Unit *);
        void AttackStart(Unit *);
        void EnterEvadeMode();
        void HealBy(Unit *healer, uint32 amount_healed);
        void DamageInflict(Unit *healer, uint32 amount_healed);
        bool IsVisible(Unit *) const;

        void UpdateAI(const uint32);
        static int Permissible(const Creature *);

    private:
        bool _isVisible(Unit *) const;
        bool _needToStop(void) const;
        void _stopAttack(void);

        Creature &i_pet;
        uint64 i_victimGuid;
        TimeTracker i_tracker;
        //uint32 i_RepeatAction;
};
#endif
