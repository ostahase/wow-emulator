/* 
 * Copyright (C) 2005,2006 MaNGOS <http://www.mangosproject.org/>
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

#ifndef MANGOS_UTILITIES_H
#define MANGOS_UTILITIES_H

#include "Creature.h"

#define SPIRIT_HEALER       6491

namespace MaNGOS
{
    namespace Utilities
    {
        template<class T> inline bool IsSpiritHealer(T *obj)
        {
            return false;
        }

        template<> inline bool IsSpiritHealer(Creature *obj)
        {
            return ( obj->GetUInt32Value(OBJECT_FIELD_ENTRY) == SPIRIT_HEALER );
        }
    }
}
#endif
