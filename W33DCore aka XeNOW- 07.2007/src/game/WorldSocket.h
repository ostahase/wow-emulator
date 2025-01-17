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

/// \addtogroup u2w
/// @{
/// \file

#ifndef __WORLDSOCKET_H
#define __WORLDSOCKET_H

#include "Network/TcpSocket.h"
#include "Auth/AuthCrypt.h"

enum ResponseCodes
{
    RESPONSE_SUCCESS                       = 0x00,
    RESPONSE_FAILURE                       = 0x01,
    RESPONSE_CANCELLED                     = 0x02,
    RESPONSE_DISCONNECTED                  = 0x03,
    RESPONSE_FAILED_TO_CONNECT             = 0x04,
    RESPONSE_CONNECTED                     = 0x05,
    RESPONSE_VERSION_MISMATCH              = 0x06,

    CSTATUS_CONNECTING                     = 0x07,
    CSTATUS_NEGOTIATING_SECURITY           = 0x08,
    CSTATUS_NEGOTIATION_COMPLETE           = 0x09,
    CSTATUS_NEGOTIATION_FAILED             = 0x0A,
    CSTATUS_AUTHENTICATING                 = 0x0B,

    AUTH_OK                                = 0x0C,
    AUTH_FAILED                            = 0x0D,
    AUTH_REJECT                            = 0x0E,
    AUTH_BAD_SERVER_PROOF                  = 0x0F,
    AUTH_UNAVAILABLE                       = 0x10,
    AUTH_SYSTEM_ERROR                      = 0x11,
    AUTH_BILLING_ERROR                     = 0x12,
    AUTH_BILLING_EXPIRED                   = 0x13,
    AUTH_VERSION_MISMATCH                  = 0x14,
    AUTH_UNKNOWN_ACCOUNT                   = 0x15,
    AUTH_INCORRECT_PASSWORD                = 0x16,
    AUTH_SESSION_EXPIRED                   = 0x17,
    AUTH_SERVER_SHUTTING_DOWN              = 0x18,
    AUTH_ALREADY_LOGGING_IN                = 0x19,
    AUTH_LOGIN_SERVER_NOT_FOUND            = 0x1A,
    AUTH_WAIT_QUEUE                        = 0x1B,
    AUTH_BANNED                            = 0x1C,
    AUTH_ALREADY_ONLINE                    = 0x1D,
    AUTH_NO_TIME                           = 0x1E,
    AUTH_DB_BUSY                           = 0x1F,
    AUTH_SUSPENDED                         = 0x20,
    AUTH_PARENTAL_CONTROL                  = 0x21,

    REALM_LIST_IN_PROGRESS                 = 0x22,
    REALM_LIST_SUCCESS                     = 0x23,
    REALM_LIST_FAILED                      = 0x24,
    REALM_LIST_INVALID                     = 0x25,
    REALM_LIST_REALM_NOT_FOUND             = 0x26,

    ACCOUNT_CREATE_IN_PROGRESS             = 0x27,
    ACCOUNT_CREATE_SUCCESS                 = 0x28,
    ACCOUNT_CREATE_FAILED                  = 0x29,

    CHAR_LIST_RETRIEVING                   = 0x2A,
    CHAR_LIST_RETRIEVED                    = 0x2B,
    CHAR_LIST_FAILED                       = 0x2C,

    CHAR_CREATE_IN_PROGRESS                = 0x2D,
    CHAR_CREATE_SUCCESS                    = 0x2E,
    CHAR_CREATE_ERROR                      = 0x2F,
    CHAR_CREATE_FAILED                     = 0x30,
    CHAR_CREATE_NAME_IN_USE                = 0x31,
    CHAR_CREATE_DISABLED                   = 0x32,
    CHAR_CREATE_PVP_TEAMS_VIOLATION        = 0x33,
    CHAR_CREATE_SERVER_LIMIT               = 0x34,
    CHAR_CREATE_ACCOUNT_LIMIT              = 0x35,
    CHAR_CREATE_SERVER_QUEUE               = 0x36,
    CHAR_CREATE_ONLY_EXISTING              = 0x37,
    CHAR_CREATE_EXPANSION                  = 0x38,

    CHAR_DELETE_IN_PROGRESS                = 0x39,
    CHAR_DELETE_SUCCESS                    = 0x3A,
    CHAR_DELETE_FAILED                     = 0x3B,
    CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER = 0x3C,

    CHAR_LOGIN_IN_PROGRESS                 = 0x3D,
    CHAR_LOGIN_SUCCESS                     = 0x3E,
    CHAR_LOGIN_NO_WORLD                    = 0x3F,
    CHAR_LOGIN_DUPLICATE_CHARACTER         = 0x40,
    CHAR_LOGIN_NO_INSTANCES                = 0x41,
    CHAR_LOGIN_FAILED                      = 0x42,
    CHAR_LOGIN_DISABLED                    = 0x43,
    CHAR_LOGIN_NO_CHARACTER                = 0x44,
    CHAR_LOGIN_LOCKED_FOR_TRANSFER         = 0x45,
    CHAR_LOGIN_LOCKED_BY_BILLING           = 0x46, 

    CHAR_NAME_NO_NAME                      = 0x47,
    CHAR_NAME_TOO_SHORT                    = 0x48,
    CHAR_NAME_TOO_LONG                     = 0x49,
    CHAR_NAME_INVALID_CHARACTER            = 0x4A,
    CHAR_NAME_MIXED_LANGUAGES              = 0x4B,
    CHAR_NAME_PROFANE                      = 0x4C,
    CHAR_NAME_RESERVED                     = 0x4D,
    CHAR_NAME_INVALID_APOSTROPHE           = 0x4E,
    CHAR_NAME_MULTIPLE_APOSTROPHES         = 0x4F,
    CHAR_NAME_THREE_CONSECUTIVE            = 0x50,
    CHAR_NAME_INVALID_SPACE                = 0x51,
    CHAR_NAME_CONSECUTIVE_SPACES           = 0x52,
    CHAR_NAME_FAILURE                      = 0x53,
    CHAR_NAME_SUCCESS                      = 0x54, 
};

class WorldPacket;
class SocketHandler;
class WorldSession;

/// Handle connection with the client software
class WorldSocket : public TcpSocket
{
    public:
        WorldSocket(SocketHandler&);
        ~WorldSocket();

        void SendPacket(WorldPacket* packet);
        void CloseSocket();

        void OnAccept();
        void OnRead();
        void OnDelete();

        void Update(time_t diff);

    protected:
        void _HandleAuthSession(WorldPacket& recvPacket);
        void _HandlePing(WorldPacket& recvPacket);
        void SendAuthWaitQue(uint32 PlayersInQue);

    private:
        AuthCrypt _crypt;
        uint32 _seed;
        uint32 _cmd;
        uint16 _remaining;
        WorldSession* _session;

        ZThread::LockedQueue<WorldPacket*,ZThread::FastMutex> _sendQueue;

        uint32 m_LastPingMSTime;
        uint32 m_OverSpeedPings;

        // internal checks
        void SizeError(WorldPacket const& packet, uint32 size) const;
};
#endif
/// @}
