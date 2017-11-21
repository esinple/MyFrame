#pragma once
#include <stdint.h>
#include <cassert>
#ifdef WIN_SYSTEM
#include <WinSock2.h>
#include <windows.h>
#else
//#include <libkern/OSByteOrder.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif


struct MPMsgHeadBase
{
	enum MP_Head
	{
		MP_HEAD_LENGTH = 6,
	};

	virtual int EnCode(char* strData) = 0;
	virtual int DeCode(const char* strData) = 0;

	virtual uint16_t GetMsgID() const = 0;
	virtual void SetMsgID(uint16_t nMsgID) = 0;

	virtual uint32_t GetBodyLength() const = 0;
	virtual void SetBodyLength(uint32_t nLength) = 0;

	int64_t MP_HTONLL(int64_t nData)
	{
#ifdef WIN_SYSTEM
		return htonll(nData);
#else
		return htobe64(nData);
#endif
	}

	int64_t MP_NTOHLL(int64_t nData)
	{
#ifdef WIN_SYSTEM
		return ntohll(nData);
#else
		return be64toh(nData);
#endif
	}

	int32_t MP_HTONL(int32_t nData)
	{
#ifdef WIN_SYSTEM
		return htonl(nData);
#else
		return htobe32(nData);
#endif
	}

	int32_t MP_NTOHL(int32_t nData)
	{
#ifdef WIN_SYSTEM
		return ntohl(nData);
#else
		return be32toh(nData);
#endif
	}

	int16_t MP_HTONS(int16_t nData)
	{
#ifdef WIN_SYSTEM
		return htons(nData);
#else
		return htobe16(nData);
#endif
	}

	int16_t MP_NTOHS(int16_t nData)
	{
#ifdef WIN_SYSTEM
		return ntohs(nData);
#else
		return be16toh(nData);
#endif
	}

};



class MPMsgHead : public MPMsgHeadBase
{
public:
	MPMsgHead()
	{
		munSize = 0;
		munMsgID = 0;
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual int EnCode(char* strData)
	{
		uint32_t nOffset = 0;

		uint16_t nMsgID = MP_HTONS(munMsgID);
		memcpy(strData + nOffset, (void*)(&nMsgID), sizeof(munMsgID));
		nOffset += sizeof(munMsgID);

		uint32_t nPackSize = munSize + MP_HEAD_LENGTH;
		uint32_t nSize = MP_HTONL(nPackSize);
		memcpy(strData + nOffset, (void*)(&nSize), sizeof(munSize));
		nOffset += sizeof(munSize);

		if (nOffset != MP_HEAD_LENGTH)
		{
			assert(0);
		}

		return nOffset;
	}

	// Message Head[ MsgID(2) | MsgSize(4) ]
	virtual int DeCode(const char* strData)
	{
		uint32_t nOffset = 0;

		uint16_t nMsgID = 0;
		memcpy(&nMsgID, strData + nOffset, sizeof(munMsgID));
		munMsgID = MP_NTOHS(nMsgID);
		nOffset += sizeof(munMsgID);

		uint32_t nPackSize = 0;
		memcpy(&nPackSize, strData + nOffset, sizeof(munSize));
		munSize = MP_NTOHL(nPackSize) - MP_HEAD_LENGTH;
		nOffset += sizeof(munSize);

		if (nOffset != MP_HEAD_LENGTH)
		{
			assert(0);
		}

		return nOffset;
	}

	virtual uint16_t GetMsgID() const
	{
		return munMsgID;
	}
	virtual void SetMsgID(uint16_t nMsgID)
	{
		munMsgID = nMsgID;
	}

	virtual uint32_t GetBodyLength() const
	{
		return munSize;
	}
	virtual void SetBodyLength(uint32_t nLength)
	{
		munSize = nLength;
	}
protected:
	uint32_t munSize;
	uint16_t munMsgID;
};