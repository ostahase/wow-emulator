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

#include "ObjectGridLoader.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "MapManager.h"
#include "RedZoneDistrict.h"
#include "Creature.h"
#include "GameObject.h"
#include "DynamicObject.h"
#include "Corpse.h"

class MANGOS_DLL_DECL ObjectGridRespawnMover
{
    public:
        ObjectGridRespawnMover() {}

        void Move(GridType &grid);

        template<class T> void Visit(std::map<OBJECT_HANDLE, T *> &m) {}
        template<class T> void Visit(std::map<OBJECT_HANDLE, CountedPtr<T> > &m) {}
        void Visit(CreatureMapType &m);
};

void
ObjectGridRespawnMover::Move(GridType &grid)
{
    TypeContainerVisitor<ObjectGridRespawnMover, GridTypeMapContainer > mover(*this);
    grid.Visit(mover);
}

void
ObjectGridRespawnMover::Visit(CreatureMapType &m)
{
    if( m.size() == 0 )
        return;

    // creature in unloading grid can have respawn point in another grid
    // if it will be unloaded then it will not respawn in original grid until unload/load original grid
    // move to respwn point to prevent this case. For player view in respawn grid this wll be normal respawn.
    for(CreatureMapType::iterator iter=m.begin(), next; iter != m.end(); iter = next)
    {
        next = iter; ++next;

        Creature * c = iter->second;

        assert(!c->isPet() && "ObjectGridRespawnMover don't must be called for pets");

        Cell const& cur_cell  = c->GetCurrentCell();

        float resp_x, resp_y, resp_z;
        c->GetRespawnCoord(resp_x, resp_y, resp_z);
        CellPair resp_val = MaNGOS::ComputeCellPair(resp_x, resp_y);
        Cell resp_cell = RedZone::GetZone(resp_val);

        if(cur_cell.DiffGrid(resp_cell))
        {
            MapManager::Instance().GetMap(c->GetMapId(), c)->CreatureRespawnRelocation(c);
            // false result ignored: will be unload with other creatures at grid
        }
    }
}

template<class T> void addUnitState(CountedPtr<T> &obj, CellPair const& cell_pair)
{
}

template<class T> void addUnitState(T *obj, CellPair const& cell_pair)
{
}

template<> void addUnitState(Creature *obj, CellPair const& cell_pair)
{
    Cell cell = RedZone::GetZone(cell_pair);

    obj->SetCurrentCell(cell);
    if( obj->isSpiritHealer())
        obj->setDeathState(DEAD);
}

template <class T>
void LoadHelper(CellGuidSet const& guid_set, CellPair &cell, std::map<OBJECT_HANDLE, T*> &m, uint32 &count, Map* map)
{
    bool generateGuid = map->Instanceable();

    for(CellGuidSet::const_iterator i_guid = guid_set.begin(); i_guid != guid_set.end(); ++i_guid)
    {
        T* obj = new T(NULL);
        uint32 guid = *i_guid;
        //sLog.outString("DEBUG: LoadHelper from table: %s for (guid: %u) Loading",table,guid);
        if(!obj->LoadFromDB(guid, map->GetInstanceId()))
        {
            //delete obj;
            continue;
        }

        obj->SetInstanceId(map->GetInstanceId());
        m[obj->GetGUID()] = obj;

        addUnitState(obj,cell);
        obj->AddToWorld();
        ++count;

    }
}

void LoadHelper(CellCorpseSet const& cell_corpses, CellPair &cell, std::map<OBJECT_HANDLE, CountedPtr<Corpse> > &m, uint32 &count, Map* map)
{
    if(cell_corpses.empty())
        return;

    bool generateGuid = map->Instanceable();
    CountedPtr<Corpse> obj;

    for(CellCorpseSet::const_iterator itr = cell_corpses.begin(); itr != cell_corpses.end(); ++itr)
    {
        if(itr->second != map->GetInstanceId())
            continue;

        uint32 player_guid = itr->first;

        obj = ObjectAccessor::Instance().GetCorpseForPlayerGUID(player_guid);
        if(!obj)
            continue;

        m[obj->GetGUID()] = obj;

        addUnitState(obj,cell);
        obj->AddToWorld();
        ++count;
    }
}

void
ObjectGridLoader::Visit(GameObjectMapType &m)
{
    uint32 x = (i_cell.GridX()*MAX_NUMBER_OF_CELLS) + i_cell.CellX();
    uint32 y = (i_cell.GridY()*MAX_NUMBER_OF_CELLS) + i_cell.CellY();
    CellPair cell_pair(x,y);
    uint32 cell_id = (cell_pair.y_coord*TOTAL_NUMBER_OF_CELLS_PER_MAP) + cell_pair.x_coord;

    CellObjectGuids const& cell_guids = objmgr.GetCellObjectGuids(i_map->GetId(), cell_id);

    LoadHelper(cell_guids.gameobjects, cell_pair, m, i_gameObjects, i_map);
}

void
ObjectGridLoader::Visit(CreatureMapType &m)
{
    uint32 x = (i_cell.GridX()*MAX_NUMBER_OF_CELLS) + i_cell.CellX();
    uint32 y = (i_cell.GridY()*MAX_NUMBER_OF_CELLS) + i_cell.CellY();
    CellPair cell_pair(x,y);
    uint32 cell_id = (cell_pair.y_coord*TOTAL_NUMBER_OF_CELLS_PER_MAP) + cell_pair.x_coord;

    CellObjectGuids const& cell_guids = objmgr.GetCellObjectGuids(i_map->GetId(), cell_id);

    LoadHelper(cell_guids.creatures, cell_pair, m, i_gameObjects, i_map);
}

void
ObjectGridLoader::Visit(CorpseMapType &m)
{
    uint32 x = (i_cell.GridX()*MAX_NUMBER_OF_CELLS) + i_cell.CellX();
    uint32 y = (i_cell.GridY()*MAX_NUMBER_OF_CELLS) + i_cell.CellY();
    CellPair cell_pair(x,y);
    uint32 cell_id = (cell_pair.y_coord*TOTAL_NUMBER_OF_CELLS_PER_MAP) + cell_pair.x_coord;

    CellObjectGuids const& cell_guids = objmgr.GetCellObjectGuids(i_map->GetId(), cell_id);
    LoadHelper(cell_guids.corpses, cell_pair, m, i_corpses, i_map);
}

void
ObjectGridLoader::Load(GridType &grid)
{
    TypeContainerVisitor<ObjectGridLoader, GridTypeMapContainer > loader(*this);
    grid.Visit(loader);
}

void ObjectGridLoader::LoadN(void)
{
    i_gameObjects = 0; i_creatures = 0; i_corpses = 0;
    i_cell.data.Part.cell_y = 0;
    for(unsigned int x=0; x < MAX_NUMBER_OF_CELLS; ++x)
    {
        i_cell.data.Part.cell_x = x;
        for(unsigned int y=0; y < MAX_NUMBER_OF_CELLS; ++y)
        {
            i_cell.data.Part.cell_y = y;
            GridLoader<Player, AllWorldObjectTypes, AllGridObjectTypes> loader;
            loader.Load(i_grid(x, y), *this);
        }
    }
    sLog.outDebug("%u GameObjects, %u Creatures, and %u Corpses/Bones loaded for grid %u on map %u", i_gameObjects, i_creatures, i_corpses,i_grid.GetGridId(), i_map->GetId());
}

void ObjectGridUnloader::MoveToRespawnN()
{
    for(unsigned int x=0; x < MAX_NUMBER_OF_CELLS; ++x)
    {
        for(unsigned int y=0; y < MAX_NUMBER_OF_CELLS; ++y)
        {
            ObjectGridRespawnMover mover;
            mover.Move(i_grid(x, y));
        }
    }
}

void
ObjectGridUnloader::Unload(GridType &grid)
{
    TypeContainerVisitor<ObjectGridUnloader, GridTypeMapContainer > unloader(*this);
    grid.Visit(unloader);
}

template<class T>
void
ObjectGridUnloader::Visit(std::map<OBJECT_HANDLE, T *> &m)
{
    if( m.size() == 0 )
        return;

    for(typename std::map<OBJECT_HANDLE, T* >::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        // if option set then object already saved at this moment
        if(!sWorld.getConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATLY))
            iter->second->SaveRespawnTime();
        delete iter->second;
    }

    m.clear();
}

template<class T>
void
ObjectGridUnloader::Visit(std::map<OBJECT_HANDLE, CountedPtr<T> > &m)
{
    if( m.size() == 0 )
        return;

    for(typename std::map<OBJECT_HANDLE, CountedPtr<T> >::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        // if option set then object already saved at this moment
        if(!sWorld.getConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATLY))
            iter->second->SaveRespawnTime();
        //delete iter->second;
    }

    m.clear();
}

template<>
void
ObjectGridUnloader::Visit(CreatureMapType &m)
{
    if( m.size() == 0 )
        return;

    // remove all cross-reference before deleting
    for(CreatureMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
        iter->second->CleanupCrossRefsBeforeDelete();

    for(CreatureMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        // if option set then object already saved at this moment
        if(!sWorld.getConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATLY))
            iter->second->SaveRespawnTime();
        delete iter->second;
    }

    m.clear();
}

void
ObjectGridStoper::Stop(GridType &grid)
{
    TypeContainerVisitor<ObjectGridStoper, GridTypeMapContainer > stoper(*this);
    grid.Visit(stoper);
}

void
ObjectGridStoper::Visit(CreatureMapType &m)
{
    if( m.size() == 0 )
        return;

    // stop any fights at grid de-activation
    for(CreatureMapType::iterator iter=m.begin(); iter != m.end(); ++iter)
    {
        iter->second->CombatStop(true);
        iter->second->DeleteThreatList();
    }
}

template void ObjectGridUnloader::Visit(GameObjectMapType &m);
template void ObjectGridUnloader::Visit(DynamicObjectMapType &m);
template void ObjectGridUnloader::Visit(std::map<OBJECT_HANDLE, CountedPtr<Corpse> > &m);
