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

#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "Unit.h"

void WorldSession::HandleAuctionListBidderItems( WorldPacket & recv_data )
{
	uint64 guid;
	uint32 unk1;
	float unknownAuction;

	recv_data >> guid;
	recv_data >> unk1;
	uint32 unk2 = unk1;

	WorldPacket data;

	data.Initialize( SMSG_AUCTION_BIDDER_LIST_RESULT );
	uint32 cnt = 0;
	Player *pl = GetPlayer();
	std::list<bidentry*>::iterator itr;
	for (itr = pl->GetBidBegin(); itr != pl->GetBidEnd(); itr++)
	{
		AuctionEntry *ae = objmgr.GetAuction((*itr)->AuctionID);
		if ((unk1-- <= 0) && (ae) && (ae->auctioneer = GUID_LOPART(guid)))
		{
			cnt++;
		}
	}
	if ((cnt-unk2) < 50 )
	{
		data << uint32(cnt-unk2);
	}
	else
	{
		data << uint32(50);
	}
	uint32 cnter = 1;
	for (itr = pl->GetBidBegin(); itr != pl->GetBidEnd(); itr++)
	{
		AuctionEntry *ae = objmgr.GetAuction((*itr)->AuctionID);
		if ((cnter > unk2) && (cnter <= (unk2+50)) && (ae->auctioneer = GUID_LOPART(guid)) && (ae))
		{
			data << ae->Id;
			Item *it = objmgr.GetAItem(ae->item);
			data << it->GetUInt32Value(OBJECT_FIELD_ENTRY);
			data << uint32(1);
			data << uint32(0);
			data << uint32(0);
			data << uint32(1);
			data << uint32(0);
			data << it->GetUInt64Value(ITEM_FIELD_OWNER);
			data << ae->bid;
			data << uint32(0);
			data << ae->buyout;
			data << uint32((ae->time - time(NULL)) * 1000);
			data << uint64(0);
			data << ae->bid;
		}
		cnter++;
	}
	data << cnt;
	SendPacket(&data);
}
void WorldSession::HandleAuctionPlaceBid( WorldPacket & recv_data )
{
	uint64 auctioneer;
	uint32 auction,price;
	WorldPacket data;
	recv_data >> auctioneer;
	recv_data >> auction >> price;
	AuctionEntry *ah = objmgr.GetAuction(auction);
	Player *pl = GetPlayer();
	if ((ah) && (ah->owner != pl->GetGUIDLow()))
	{
		if (((price < ah->buyout) || (ah->buyout == 0)))
		{
			if (ah->bidder != 0)
			{
				printf("player %u lost a bid\n",ah->bidder);
				Mail* n = new Mail;
				n->messageID = objmgr.GenerateMailID();
				n->sender = ah->owner;
				n->reciever = ah->bidder;
				n->subject = "You have lost a bid";
				n->body = "";
				n->item = 0;
				n->money = ah->bid;
				n->time = time(NULL) + (30 * 3600);
				n->COD = 0;
				n->checked = 0;
				uint64 rc;
				GUID_LOPART(rc) = ah->bidder;
				GUID_HIPART(rc) = 0;
				Player *rpl = objmgr.GetObject<Player>(rc);
				std::stringstream ss;
				ss << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
					n->messageID << ", " << n->sender << ", " << n->reciever << ",' " << n->subject.c_str() << "' ,' " <<
					n->body.c_str() << "', " << n->item << ", " << n->time << ", " << n->money << ", " << n->COD << ", " << n->checked << " )";
				sDatabase.Execute( ss.str().c_str( ) );
				if (rpl)
					rpl->AddMail(n);
			}
			ah->bidder = pl->GetGUIDLow();
			ah->bid = price;
			//objmgr.RemoveAuction(ah->Id);
			//objmgr.AddAuction(ah);
			// adding bid to player
			bidentry *be = new bidentry;
			be->AuctionID = auction;
			be->amt = price;
			pl->SetUInt32Value(PLAYER_FIELD_COINAGE,(pl->GetUInt32Value(PLAYER_FIELD_COINAGE) - price));
			pl->AddBid(be);
			uint64 guid = auctioneer;
			//sending the updated bidder list
			data.Initialize( SMSG_AUCTION_BIDDER_LIST_RESULT );
			uint32 cnt = 0;
			std::list<bidentry*>::iterator itr;
			for (itr = pl->GetBidBegin(); itr != pl->GetBidEnd(); itr++)
			{
				AuctionEntry *ae = objmgr.GetAuction((*itr)->AuctionID);
				if (ae->auctioneer = GUID_LOPART(guid))
				{
					cnt++;
				}
			}
			if (cnt < 51)
			{
				data << cnt;
			}
			else
			{
				data << uint32(50);
			}
			uint32 cnter = 1;
			for (itr = pl->GetBidBegin(); itr != pl->GetBidEnd(); itr++)
			{
				AuctionEntry *ae = objmgr.GetAuction((*itr)->AuctionID);
				if ((ae->auctioneer = GUID_LOPART(guid)) && (cnter < 33))
				{
					data << ae->Id;
					Item *it = objmgr.GetAItem(ae->item);
					data << it->GetUInt32Value(OBJECT_FIELD_ENTRY);
					data << uint32(0);
					data << uint32(0);
					data << uint32(0);
					data << uint32(1);
					data << uint32(0);
					data << it->GetUInt64Value(ITEM_FIELD_OWNER);
					data << ae->bid;
					data << uint32(0);
					data << ae->buyout;
					data << uint32((ae->time - time(NULL)) * 1000);
					data << uint64(0);
					data << ae->bid;
					cnter++;
				}
			}
			data << cnt;
			SendPacket(&data);
			data.clear();
			data.Initialize( SMSG_AUCTION_LIST_RESULT );
			data << uint32(0);
			data << uint32(0);
			SendPacket(&data);
		}
		else
		{
			pl->SetUInt32Value(PLAYER_FIELD_COINAGE,(pl->GetUInt32Value(PLAYER_FIELD_COINAGE) - ah->buyout));
			Mail *m = new Mail;
			m->messageID = objmgr.GenerateMailID();
			m->sender = ah->owner;
			m->reciever = pl->GetGUIDLow();
			m->subject = "You won an item!";
			m->body = "";
			m->checked = 0;
			m->COD = 0;
			m->money = 0;
			m->item = ah->item;
			m->time = time(NULL) + (29 * 3600);

			Item *it = objmgr.GetAItem(ah->item);

			objmgr.AddMItem(it);
			std::stringstream ss;
			ss << "INSERT INTO mailed_items (guid, data) VALUES ("
				<< it->GetGUIDLow() << ", '"; // TODO: use full guids
			for(uint16 i = 0; i < it->GetValuesCount(); i++ )
			{
				ss << it->GetUInt32Value(i) << " ";
			}
			ss << "' )";
			sDatabase.Execute( ss.str().c_str() );

			std::stringstream md;
			md << "DELETE FROM mail WHERE mailID = " << m->messageID; // TODO: use full guids
			sDatabase.Execute( md.str().c_str( ) );

			std::stringstream mi;
			mi << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
				m->messageID << ", " << pl->GetGUIDLow() << ", " << m->reciever << ",' " << m->subject.c_str() << "' ,' " <<
				m->body.c_str() << "', " << m->item << ", " << m->time << ", " << m->money << ", " << 0 << ", " << m->checked << " )";
			sDatabase.Execute( mi.str().c_str( ) );

			uint64 rcpl;
			GUID_LOPART(rcpl) = m->reciever;
			GUID_HIPART(rcpl) = 0;
			Player *rpl = objmgr.GetObject<Player>(rcpl);
			if (rpl)
				rpl->AddMail(m);


			if (ah->bidder != 0)
			{
				printf("player %u lost a bid\n",ah->bidder);
				Mail* n = new Mail;
				n->messageID = objmgr.GenerateMailID();
				n->sender = ah->owner;
				n->reciever = ah->bidder;
				n->subject = "You have lost a bid";
				n->body = "";
				n->item = 0;
				n->money = ah->bid;
				n->time = time(NULL) + (30 * 3600);
				n->COD = 0;
				n->checked = 0;
				uint64 rc;
				GUID_LOPART(rc) = ah->bidder;
				GUID_HIPART(rc) = 0;
				Player *rpl = objmgr.GetObject<Player>(rc);
				std::stringstream ss;
				ss << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
					n->messageID << ", " << n->sender << ", " << n->reciever << ",' " << n->subject.c_str() << "' ,' " <<
					n->body.c_str() << "', " << n->item << ", " << n->time << ", " << n->money << ", " << n->COD << ", " << n->checked << " )";
				sDatabase.Execute( ss.str().c_str( ) );
				if (rpl)
					rpl->AddMail(n);
			}
			std::stringstream delinvq;
			std::stringstream id;
			std::stringstream bd;

			delinvq << "DELETE FROM auctionhouse WHERE itemowner = " << ah->owner; // TODO: use full guids				
			sDatabase.Execute( delinvq.str().c_str( ) );

			id << "DELETE FROM auctioned_items WHERE guid = " << ah->item; // TODO: use full guids				
			sDatabase.Execute( id.str().c_str( ) );

			bd << "DELETE FROM bids WHERE Id = " << ah->Id; // TODO: use full guids				
			sDatabase.Execute( bd.str().c_str( ) );
			data.Initialize( SMSG_AUCTION_LIST_RESULT );
			data << uint32(0);
			data << uint32(0);
			SendPacket(&data);

			Mail *mn = new Mail;
			mn->messageID = objmgr.GenerateMailID();
			mn->sender = pl->GetGUIDLow();
			mn->reciever = ah->owner;
			mn->subject = "Your item sold!";
			mn->body = "";
			mn->checked = 0;
			mn->COD = 0;
			mn->money = ah->buyout;
			mn->item = 0;
			mn->time = time(NULL) + (29 * 3600);
			std::stringstream mdn;
			mdn << "DELETE FROM mail WHERE mailID = " << mn->messageID; // TODO: use full guids
			sDatabase.Execute( mdn.str().c_str( ) );

			std::stringstream min;
			min << "INSERT INTO mail (mailId,sender,reciever,subject,body,item,time,money,COD,checked) VALUES ( " <<
				mn->messageID << ", " << mn->sender << ", " << mn->reciever << ",' " << mn->subject.c_str() << "' ,' " <<
				mn->body.c_str() << "', " << mn->item << ", " << mn->time << ", " << mn->money << ", " << 0 << ", " << mn->checked << " )";
			sDatabase.Execute( min.str().c_str( ) );

			uint64 rcpln;
			GUID_LOPART(rcpln) = mn->reciever;
			GUID_HIPART(rcpln) = 0;
			Player *rpln = objmgr.GetObject<Player>(rcpln);
			if (rpln)
			{
				rpln->AddMail(mn);
			}
			objmgr.RemoveAItem(ah->item);
			objmgr.RemoveAuction(ah->Id);
		}
	}
}
void WorldSession::HandleAuctionSellItem( WorldPacket & recv_data )
{
	uint64 auctioneer, item;
	uint32 etime, bid, buyout;
	recv_data >> auctioneer >> item;
	recv_data >> bid >> buyout >> etime;
	Player *pl = GetPlayer();
	AuctionEntry *AH = new AuctionEntry;
	AH->auctioneer = GUID_LOPART(auctioneer);
	AH->item = GUID_LOPART(item);
	AH->owner = pl->GetGUIDLow();
	AH->bid = bid;
	AH->bidder = 0;
	AH->buyout = buyout;
	time_t base = time(NULL);
	AH->time = ((time_t)(etime * 60)) + base;
	//AH->time = base + 300;
	AH->Id = objmgr.GenerateAuctionID();
	Log::getSingleton().outString("selling item %u to auctioneer %u with inital bid %u with buyout %u and with time %u (in minutes)",GUID_LOPART(item),GUID_LOPART(auctioneer),bid,buyout,time);
	objmgr.AddAuction(AH);
	uint32 slot = pl->GetSlotByItemGUID(item);
	Item *it = pl->GetItemBySlot((uint8)slot);
	objmgr.AddAItem(it);

	std::stringstream ss;
	ss << "INSERT INTO auctioned_items (guid, data) VALUES ("
		<< it->GetGUIDLow() << ", '"; // TODO: use full guids
	for(uint16 i = 0; i < it->GetValuesCount(); i++ )
	{
		ss << it->GetUInt32Value(i) << " ";
	}
	ss << "' )";
	sDatabase.Execute( ss.str().c_str() );

	pl->RemoveItemFromSlot((uint8)slot);
	WorldPacket data;
	ObjectMgr::AuctionEntryMap::iterator itr;
	uint32 cnt = 0;
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		if ((itr->second->auctioneer == GUID_LOPART(auctioneer)) && (itr->second->owner == pl->GetGUIDLow()))
		{
			cnt++;
		}
	}
	Log::getSingleton().outString("sending owner list with %u items",cnt);
	data.Initialize( SMSG_AUCTION_OWNER_LIST_RESULT );
	if (cnt < 51)
	{
		data << uint32(cnt);
	}
	else
	{
		data << uint32(50);
	}
	uint32 cnter = 1;
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		if ((itr->second->auctioneer == GUID_LOPART(auctioneer)) && (itr->second->owner == pl->GetGUIDLow()) && (cnter < 51))
		{
			AuctionEntry *Aentry = itr->second;
			data << Aentry->Id;
			Item *it = objmgr.GetAItem(Aentry->item);
			data << it->GetUInt32Value(OBJECT_FIELD_ENTRY);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(1);
			data << uint32(0);
			data << it->GetUInt64Value(ITEM_FIELD_OWNER);
			data << Aentry->bid;
			data << uint32(0);
			data << Aentry->buyout;
			time_t base = time(NULL);
			data << uint32((Aentry->time - base) * 1000);
			data << uint64(0);
			data << Aentry->bid;
			cnter++;
		}
	}
	data << cnt;
	SendPacket(&data);

}
void WorldSession::HandleAuctionListOwnerItems( WorldPacket & recv_data )
{
	WorldPacket data;
	uint64 auctioneer;
	recv_data >> auctioneer;
	ObjectMgr::AuctionEntryMap::iterator itr;
	uint32 cnt = 0;
	Player *pl = GetPlayer();
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		if ((itr->second->auctioneer == GUID_LOPART(auctioneer)) && (itr->second->owner == pl->GetGUIDLow()))
		{
			cnt++;
		}
	}
	Log::getSingleton().outString("sending owner list with %u items",cnt);
	data.Initialize( SMSG_AUCTION_OWNER_LIST_RESULT );
	if (cnt < 51)
	{
		data << uint32(cnt);
	}
	else
	{
		data << uint32(50);
	}
	uint32 cnter = 1;
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		if ((itr->second->auctioneer == GUID_LOPART(auctioneer)) && (itr->second->owner == pl->GetGUIDLow()) && (cnter < 51))
		{
			AuctionEntry *Aentry = itr->second;
			data << Aentry->Id;
			Log::getSingleton().outString("getting item with id %u",Aentry->item);
			Item *it = objmgr.GetAItem(Aentry->item);
			data << it->GetUInt32Value(OBJECT_FIELD_ENTRY);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(1);
			data << uint32(0);
			data << it->GetUInt64Value(ITEM_FIELD_OWNER);
			data << Aentry->bid;
			data << uint32(0);
			data << Aentry->buyout;
			data << uint32((Aentry->time - time(NULL)) * 1000);
			data << uint32(Aentry->bidder);
			data << uint32(0);
			data << Aentry->bid;
			cnter++;
		}
	}
	data << cnt;
	SendPacket(&data);
}
void WorldSession::HandleAuctionListItems( WorldPacket & recv_data )
{
	std::string auctionString;
	uint8 levelRange1, levelRange2, usableCheck;
	uint32 cnt, guidhigh, guidlow, unk1, auctionSlotID, auctionMainCatagory, auctionSubCatagory, rarityCheck, unk2;

	recv_data >> guidhigh >> guidlow;
	recv_data >> unk1; 
	recv_data >> auctionString;
	recv_data >> levelRange1 >> levelRange2;
	recv_data >> auctionSlotID >> auctionMainCatagory >> auctionSubCatagory;
	recv_data >> rarityCheck >> usableCheck;
	unk2 = unk1;

	Log::getSingleton( ).outBasic("%s",auctionString.c_str( ) );
	Log::getSingleton( ).outBasic("\n unkAuction = %u\n Level Start = %u\n Level End = %u\n auctionSlotID = %u\n auctionMainCatagory = %u\n auctionSubCatagory = %u\n rarityCheck = %u\n usableCheck = %u\n", unk1, levelRange1, levelRange2, auctionSlotID, auctionMainCatagory, auctionSubCatagory, rarityCheck, usableCheck);

	WorldPacket data;
	ObjectMgr::AuctionEntryMap::iterator itr;
	cnt = 0;
	if (levelRange2 == 0)
	{
		levelRange2 = 100;
	}
	uint32 tempcat1, tempcat2, temprarity, tempslot;
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		AuctionEntry *Aentry = itr->second;
		Item *it = objmgr.GetAItem(Aentry->item);
		tempcat1 = auctionMainCatagory;
		tempcat2 = auctionSubCatagory;
		temprarity = rarityCheck;
		tempslot = auctionSlotID;
		if (auctionMainCatagory == (0xffffffff))
		{
			auctionMainCatagory = it->GetProto()->Class;
		}
		if (auctionSlotID == (0xffffffff))
		{
			auctionSlotID = it->GetProto()->InventoryType;
		}
		if (rarityCheck == (0xffffffff))
		{
			rarityCheck = it->GetProto()->Quality;
		}
		if (auctionSubCatagory == (0xffffffff))
		{
			auctionSubCatagory = it->GetProto()->SubClass;
		}
		if ((unk1-- <= 0) && (it->GetProto()->InventoryType == auctionSlotID) &&(it->GetProto()->Quality == rarityCheck) && (it->GetProto()->ItemLevel >= levelRange1) && (it->GetProto()->ItemLevel <= levelRange2) && (it->GetProto()->Class == auctionMainCatagory) && (it->GetProto()->SubClass == auctionSubCatagory))
		{
			cnt++;
		}
		auctionMainCatagory = tempcat1;
		auctionSubCatagory = tempcat2;
		rarityCheck = temprarity;
		auctionSlotID = tempslot;
		tempcat1 = NULL;
		tempcat2 = NULL;
		temprarity = NULL;
		tempslot = NULL;
	}
	data.Initialize( SMSG_AUCTION_LIST_RESULT );
	if ((cnt-unk2) < 50 )
	{
		data << uint32(cnt-unk2);
	}
	else
	{
		data << uint32(50);
	}
	uint32 cnter = 1;
	for (itr = objmgr.GetAuctionsBegin();itr != objmgr.GetAuctionsEnd();itr++)
	{
		AuctionEntry *Aentry = itr->second;
		Item *it = objmgr.GetAItem(Aentry->item);
		tempcat1 = auctionMainCatagory;
		tempcat2 = auctionSubCatagory;
		temprarity = rarityCheck;
		tempslot = auctionSlotID;
		if (auctionMainCatagory == (0xffffffff))
		{
			auctionMainCatagory = it->GetProto()->Class;
		}
		if (auctionSubCatagory == (0xffffffff))
		{
			auctionSubCatagory = it->GetProto()->SubClass;
		}
		if (rarityCheck == (0xffffffff))
		{
			rarityCheck = it->GetProto()->Quality;
		}
		if (auctionSlotID == (0xffffffff))
		{
			auctionSlotID = it->GetProto()->InventoryType;
		}
		if ((cnter > unk2) && (cnter <= (unk2+50)) && (it->GetProto()->InventoryType == auctionSlotID) &&(it->GetProto()->Quality == rarityCheck) && (it->GetProto()->ItemLevel >= levelRange1) && (it->GetProto()->ItemLevel <= levelRange2) && (it->GetProto()->Class == auctionMainCatagory) && (it->GetProto()->SubClass == auctionSubCatagory))
		{
			data << Aentry->Id;
			data << it->GetUInt32Value(OBJECT_FIELD_ENTRY);
			data << uint32(0);
			data << uint32(0);
			data << uint32(0);
			data << uint32(1);
			data << uint32(0);
			data << it->GetUInt64Value(ITEM_FIELD_OWNER);
			data << Aentry->bid;
			data << uint32(0);
			data << Aentry->buyout;
			data << uint32((Aentry->time - time(NULL)) * 1000);
			data << uint32(Aentry->bidder);
			data << uint32(0);
			data << Aentry->bid;
		}
		cnter++;
		auctionMainCatagory = tempcat1;
		auctionSubCatagory = tempcat2;
		rarityCheck = temprarity;
		auctionSlotID = tempslot;
		tempcat1 = NULL;
		tempcat2 = NULL;
		temprarity = NULL;
		tempslot = NULL;
	}
	data << uint32(cnt);
	SendPacket(&data);
}
