#include "MPNet.h"
#include "MPNetObject.h"
#include "MPMsg.h"
#include "tcp_conn.h"

using namespace meplay;

MPNet::MPNet() : m_nType(MP_NET_TYPE_UNKNOWN)
{
}

MPNet::~MPNet()
{
	Clear();
}

void MPNet::Clear()
{
	/*auto it = m_mNetObjects.begin();
	for (; it != m_mNetObjects.end(); ++it)
	{
		ev_real_close(it->second);
	}*/

	m_mNetObjects.clear();
}

std::map<MPSOCK, MPNetObjectPtr>& MPNet::GetAllNetObject()
{
	return m_mNetObjects;
}

MPNetObjectPtr MPNet::GetNetObject(const MPSOCK nSockIndex)
{
	auto it = m_mNetObjects.find(nSockIndex);
	if (it == m_mNetObjects.end())
	{
		return nullptr;
	}
	return it->second;
}

bool MPNet::AddNetObject(const MPSOCK nSockIndex, MPNetObjectPtr pObject)
{
	return m_mNetObjects.insert(std::map<MPSOCK, MPNetObjectPtr>::value_type(nSockIndex, pObject)).second;
}

bool MPNet::DelNetObject(const MPSOCK nSockIndex)
{
	auto it = m_mNetObjects.find(nSockIndex);
	if (it == m_mNetObjects.end())
	{
		return false;
	}
	auto pNetObject = it->second;
	pNetObject->GetConn()->Close();
	m_mNetObjects.erase(nSockIndex);

	return true;
}


MP_NET_TYPE MPNet::GetNetType()const
{
	return m_nType;
}

void MPNet::SetNetType(MP_NET_TYPE nType)
{
	m_nType = nType;
}

uint8_t MPNet::GetServerType()const
{
	return m_nServerType;
}

void MPNet::SetServerType(uint8_t nType)
{
	m_nServerType = nType;
}

bool MPNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const std::list<MPSOCK>& fdList)
{
	std::string strOutData;
    int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
    if (nAllLen == nLen + MPMsgHead::MP_Head::MP_HEAD_LENGTH)
    {
        return SendMsg(strOutData.c_str(), strOutData.length(), fdList);
    }

	return true;
}

bool MPNet::SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen, const MPSOCK nSockIndex)
{
	std::string strOutData;
	int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
	if (nAllLen == nLen + MPMsgHead::MP_Head::MP_HEAD_LENGTH)
	{
		return SendMsg(strOutData.c_str(), strOutData.length(), nSockIndex);
	}

	return true;
}

bool MPNet::SendMsgToAllWithOutHead(const int16_t nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strOutData;
	int nAllLen = EnCode(nMsgID, msg, nLen, strOutData);
	if (nAllLen == nLen + MPMsgHead::MP_Head::MP_HEAD_LENGTH)
	{
		SendMsgAll(strOutData.c_str(), (uint32_t)strOutData.length());
	}

	return true;
}

int MPNet::EnCode(const uint16_t unMsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData)
{
	MPMsgHead xHead;
	xHead.SetMsgID(unMsgID);
	xHead.SetBodyLength(unDataLen);

	char szHead[MPMsgHead::MP_Head::MP_HEAD_LENGTH] = { 0 };
	xHead.EnCode(szHead);

	strOutData.clear();
	strOutData.append(szHead, MPMsgHead::MP_Head::MP_HEAD_LENGTH);
	strOutData.append(strData, unDataLen);

	return xHead.GetBodyLength() + MPMsgHead::MP_Head::MP_HEAD_LENGTH;
}

int MPNet::DeCode(const char* strData, const uint32_t unAllLen, MPMsgHead& xHead)
{
	if (unAllLen < MPMsgHead::MP_Head::MP_HEAD_LENGTH)
	{
		return -1;
	}

	if (MPMsgHead::MP_Head::MP_HEAD_LENGTH != xHead.DeCode(strData))
	{
		return -2;
	}

	if (xHead.GetBodyLength() > (unAllLen - MPMsgHead::MP_Head::MP_HEAD_LENGTH))
	{
		return -3;
	}

	return xHead.GetBodyLength();
}

bool MPNet::SendMsg(const char* msg, const uint32_t nLen, const std::list<MPSOCK>& nSockIndex)
{
	for (auto& fd : nSockIndex)
	{
		SendMsg( msg, nLen, fd);
	}
	return true;
}

bool MPNet::SendMsg(const char* msg, const uint32_t nLen, const MPSOCK nSockIndex)
{

	if (nLen <= 0)
	{
		return false;
	}

	auto it = m_mNetObjects.find(nSockIndex);
	if (it == m_mNetObjects.end())
	{
		return false;
	}
	auto pNetObject = it->second;
	if (pNetObject == nullptr)
	{
		return false;
	}

	pNetObject->Send(msg, nLen);
	
	return true;
}

void MPNet::SendMsgAll(const char* msg, const uint32_t nLen)
{
	for (auto& fd_info : m_mNetObjects)
	{
		auto pNetObject = fd_info.second;
		if (pNetObject == nullptr)
		{
			continue;
		}
		pNetObject->Send(msg,nLen);
	}
}

void MPNet::KeepAlive()
{
}