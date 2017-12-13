#pragma once
#include <stdint.h>
#include <mutex>
#include <queue>
#include "MPNetDefine.h"
#include "MPMemoryPool.h"

struct NetMsgBuffer final
{
	uint8_t m_nNetType;
	std::list<MPSOCK> m_vSockIndex;
	uint32_t m_nMsgId;
	std::vector<unsigned char> m_vData;

	NetMsgBuffer(uint8_t nType,std::list<MPSOCK> nSockIndex,uint32_t nMsgId,const char * pData,uint32_t nLen);
	~NetMsgBuffer();
	void ReuseInit(uint8_t nType,std::list<MPSOCK> nSockIndex,uint32_t nMsgId,const char * pData,uint32_t nLen);
	void Clear();
};

typedef std::shared_ptr<NetMsgBuffer> NetMsgBufferPtr;

class NetProxyBuffer final
{
public:
	NetProxyBuffer();
	virtual ~NetProxyBuffer();
public:	
	NetMsgBufferPtr GetRecvMsg();
	void AddRecvMsg(uint8_t nNetType, MPSOCK nSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen);
	void AddRecvMsg(uint8_t nNetType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen);

	NetMsgBufferPtr GetSendMsg();
	void AddSendMsg(uint8_t nNetType, MPSOCK nSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen);
	void AddSendMsg(uint8_t nNetType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen);
private:
	MPMemoryPool<NetMsgBuffer> m_MsgPool;

	std::queue<NetMsgBufferPtr> m_vRecvBuffer;
	std::queue<NetMsgBufferPtr> m_vSendBuffer;

	std::mutex rmtx;
	std::mutex wmtx;
};