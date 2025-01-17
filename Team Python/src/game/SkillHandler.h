// Copyright (C) 2004 Team Python
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

//////////////////////////////////////////////////////////////////////
//  Skill Handler
//
//  Receives all messages with Skill management opcodes
//////////////////////////////////////////////////////////////////////

#ifndef WOWPYTHONSERVER_SKILLHANDLER_H
#define WOWPYTHONSERVER_SKILLHANDLER_H

#include "MsgHandler.h"

#define MAXTALENTID 922 
    
struct TALENT 
{ 
   uint32 TalentId; 
   uint32 TalentTab; 
   uint32 TalentRow; 
   uint32 TalentCol; 
   uint32 Class; 
   int MaxRank; 
   uint32 Spell[5]; 
}; 



class DatabaseInterface;
class SkillHandler : public MsgHandler
{
public:
	SkillHandler();
	~SkillHandler();

	void HandleMsg( wowWData & recv_data, GameClient *pClient );

protected:

};


#endif

