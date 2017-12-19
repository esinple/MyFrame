#include "NetProxyBuffer.h"

NetMsgBuffer::NetMsgBuffer(uint8_t nType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	ReuseInit(nType, std::move(vSockIndex), nMsgId, pData, nLen);
}

NetMsgBuffer::~NetMsgBuffer()
{
}

void NetMsgBuffer::ReuseInit(uint8_t nType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	m_nNetType = nType;
	m_vSockIndex = vSockIndex;
	m_nMsgId = nMsgId;
	m_vData.resize((size_t)nLen);
	memcpy(&m_vData[0], pData, nLen);
}

void NetMsgBuffer::Clear()
{
	m_nNetType = -1;
	m_vSockIndex.clear();
	m_nMsgId = -1;
	m_vData.clear();
}

NetProxyBuffer::NetProxyBuffer()
{
}

NetProxyBuffer::~NetProxyBuffer()
{
}

NetMsgBufferPtr NetProxyBuffer::GetRecvMsg()
{
	if (m_vRecvBuffer.empty())
	{
		return nullptr;
	}
	std::unique_lock<std::mutex> lck(rmtx);
	auto pBuffer = m_vRecvBuffer.front();
	m_vRecvBuffer.pop();
	lck.unlock();
	
	return pBuffer;
}

void NetProxyBuffer::AddRecvMsg(uint8_t nNetType, MPSOCK nSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	std::list<MPSOCK> vSockIndex{ nSockIndex };
	auto pBuffer = m_MsgPool.Get(nNetType, std::move(vSockIndex), nMsgId, pData, nLen);

	std::lock_guard<std::mutex> lck(rmtx);
	m_vRecvBuffer.emplace(pBuffer);
}

void NetProxyBuffer::AddRecvMsg(uint8_t nNetType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	auto pBuffer = m_MsgPool.Get(nNetType, std::move(vSockIndex), nMsgId, pData, nLen);

	std::lock_guard<std::mutex> lck(rmtx);
	m_vRecvBuffer.emplace(pBuffer);
}

NetMsgBufferPtr NetProxyBuffer::GetSendMsg()
{
	if (m_vSendBuffer.empty())
	{
		return nullptr;
	}
	std::unique_lock<std::mutex> lck(wmtx);
	auto pBuffer = m_vSendBuffer.front();
	m_vSendBuffer.pop();
	lck.unlock();

	return pBuffer;
}

void NetProxyBuffer::AddSendMsg(uint8_t nNetType, MPSOCK nSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	std::list<MPSOCK> vSockIndex{nSockIndex};
	auto pBuffer = m_MsgPool.Get(nNetType, std::move(vSockIndex), nMsgId, pData, nLen);

	std::lock_guard<std::mutex> lck(wmtx);
	m_vSendBuffer.emplace(pBuffer);
}

void NetProxyBuffer::AddSendMsg(uint8_t nNetType, std::list<MPSOCK> vSockIndex, uint32_t nMsgId, const char * pData, uint32_t nLen)
{
	auto pBuffer = m_MsgPool.Get(nNetType, std::move(vSockIndex), nMsgId, pData, nLen);

	std::lock_guard<std::mutex> lck(wmtx);
	m_vSendBuffer.emplace(pBuffer);
}