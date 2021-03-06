/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 * Copyright (C) 2010 - 2013 Eluna Lua Engine <http://emudevs.com/>
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

#ifndef GROUPMETHODS_H
#define GROUPMETHODS_H

namespace LuaGroup
{
    int GetMembers(lua_State* L, Group* group)
    {
        lua_newtable(L);
        int tbl = lua_gettop(L);
        uint32 i = 0;

        for (GroupReference* itr = group->GetFirstMember(); itr; itr = itr->next())
        {
            Player* member = itr->getSource();

            if (!member || !member->GetSession())
                continue;

            ++i;
            sEluna.Push(L, i);
            sEluna.Push(L, member);
            lua_settable(L, tbl);
        }

        lua_settop(L, tbl); // push table to top of stack
        return 1;
    }

    int GetLeaderGUID(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->GetLeaderGuid());
        return 1;
    }

    int GetLeader(lua_State* L, Group* group)
    {
        sEluna.Push(L, sObjectAccessor.FindPlayer(group->GetLeaderGuid()));
        return 1;
    }

    int GetGUID(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->GetObjectGuid());
        return 1;
    }

    int ChangeLeader(lua_State* L, Group* group)
    {
        Player* leader = sEluna.CHECK_PLAYER(L, 1);
        if (!leader)
            return 0;

        group->ChangeLeader(leader->GetObjectGuid());
        sEluna.Push(L, true);
        return 1;
    }

    int IsLeader(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        if (!player)
            if (const char* name = luaL_checkstring(L, 1))
                player = sObjectAccessor.FindPlayerByName(name);
        if (!player)
            return 0;

        sEluna.Push(L, group->IsLeader(player->GetObjectGuid()));
        return 1;
    }

    // SendPacket(packet, sendToPlayersInBattleground[, ignoreguid])
    int SendPacket(lua_State* L, Group* group)
    {
        WorldPacket* data = sEluna.CHECK_PACKET(L, 1);
        bool ignorePlayersInBg = luaL_checkbool(L, 2);
        uint64 ignore = sEluna.CHECK_ULONG(L, 3);

        if (data)
            group->BroadcastPacket(data, ignorePlayersInBg, -1, (ObjectGuid)ignore);
        return 0;
    }

    int AddInvite(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        if (!player)
            return 0;

        sEluna.Push(L, group->AddInvite(player));
        return 1;
    }

    int RemoveMember(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        bool method = luaL_optbool(L, 2, 0);
        if (!player)
            return 0;

        sEluna.Push(L, group->RemoveMember(player->GetObjectGuid(), method));
        return 1;
    }

    int Disband(lua_State* L, Group* group)
    {
        group->Disband();
        return 0;
    }

    int IsFull(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->IsFull());
        return 1;
    }

    int isLFGGroup(lua_State* L, Group* group) // TODO: Implementation
    {
        /*sEluna.Push(L, group->isLFGGroup());
        return 1;*/
        return 0; // Temporary to prevent conflicts
    }

    int isRaidGroup(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->isRaidGroup());
        return 1;
    }

    int isBGGroup(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->isBGGroup());
        return 1;
    }

    int isBFGroup(lua_State* L, Group* group) // TODO: Implementation
    {
        /*sEluna.Push(L, group->isBFGroup());
        return 1;*/
        return 0; // Temporary to prevent conflicts
    }

    int IsMember(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        if (!player)
            return 0;
        sEluna.Push(L, group->IsMember(player->GetObjectGuid()));
        return 1;
    }

    int IsAssistant(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        if (!player)
            return 0;

        sEluna.Push(L, group->IsAssistant(player->GetObjectGuid()));
        return 1;
    }

    int SameSubGroup(lua_State* L, Group* group)
    {
        Player* player1 = sEluna.CHECK_PLAYER(L, 1);
        Player* player2 = sEluna.CHECK_PLAYER(L, 2);
        if (!player1 || !player2)
            return 0;
        sEluna.Push(L, group->SameSubGroup(player1, player2));
        return 1;
    }

    int HasFreeSlotSubGroup(lua_State* L, Group* group)
    {
        uint8 subGroup = luaL_checkunsigned(L, 1);
        sEluna.Push(L, group->HasFreeSlotSubGroup(subGroup));
        return 1;
    }

    int GetMemberGUID(lua_State* L, Group* group)
    {
        const char* name = luaL_checkstring(L, 1);
        sEluna.Push(L, group->GetMemberGuid(name));
        return 1;
    }

    int GetMembersCount(lua_State* L, Group* group)
    {
        sEluna.Push(L, group->GetMembersCount());
        return 1;
    }

    int ConvertToLFG(lua_State* L, Group* group) // TODO: Implementation
    {
        // group->ConvertToLFG();
        return 0;
    }

    int ConvertToRaid(lua_State* L, Group* group)
    {
        group->ConvertToRaid();
        return 0;
    }

    int ChangeMembersGroup(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        uint8 groupID = luaL_checkunsigned(L, 2);

        if (player)
            group->ChangeMembersGroup(player->GetObjectGuid(), groupID);
        return 0;
    }

    int GetMemberGroup(lua_State* L, Group* group)
    {
        Player* player = sEluna.CHECK_PLAYER(L, 1);
        if (!player)
            return 0;

        sEluna.Push(L, group->GetMemberGroup(player->GetObjectGuid()));
        return 1;
    }
};
#endif
