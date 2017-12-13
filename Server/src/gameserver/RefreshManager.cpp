#include "RefreshManager.h"
#include "GameUser.h"
#include "GameTimeManager.h"
#include "GameUserManager.h"
#include "TimeStampManager.h"

RefreshManager::RefreshManager() : ManagerModule(eGameMgr_Refresh)
{
	AddParent(eGameMgr_TimeStamp);
}

RefreshManager::~RefreshManager()
{
}

bool RefreshManager::Awake()
{
	if (!loadUserRefreshCfg())
	{
		return false;
	}

	registerUserAllRefresh();

	return true;
}

bool RefreshManager::AfterAwake()
{
	return true;
}

bool RefreshManager::Execute()
{
	return true;
}

bool RefreshManager::BeforeShutDown()
{
	return true;
}

bool RefreshManager::ShutDown()
{
	return true;
}

void RefreshManager::GameUserLoginRefresh(GameUserPtr pGameUser)
{
	auto pTimeStampMgr = g_pGameNetProxy->GetModule<TimeStampManager>(eGameMgr_TimeStamp);
	for (auto&&[id, cfg] : m_mUserRefreshCfgs)
	{
		(void)id;
		auto nTimeStamp = pTimeStampMgr->GetTimeStamp(pGameUser->GetUID(), eTS_UserModuleRefresh, cfg.nRefreshModule);

		for(auto&&[tStart,tEnd] : cfg.vCalcTime)
		{
			(void)tEnd;
			if (nTimeStamp > tStart.CurrentSec())
			{
				continue;
			}
			pGameUser->RefreshModule(cfg.nRefreshModule);
			pTimeStampMgr->UpdateTimeStamp(pGameUser->GetUID(), eTS_UserModuleRefresh, cfg.nRefreshModule, tStart.CurrentSec());
		}
	}
}

void RefreshManager::GameUserOnlineRefresh(uint64_t id)
{
	auto pGameUserMgr = g_pGameNetProxy->GetModule<GameUserManager>(eGameMgr_GameUser);
	auto pCfg = getUserRefreshCfg(id);
	if (pCfg == nullptr)
	{
		return;
	}

	auto& now = GAME_CUR_TIME;
	for (auto&&[tStart, tEnd] : pCfg->vCalcTime)
	{
		if (tStart > now)
		{
			continue;
		}

		tStart += pCfg->nIntervalSec;
		tEnd = tStart + pCfg->nIntervalSec;
	}
	pGameUserMgr->GameUserModuleRefresh(pCfg->nRefreshModule);
}

const UserRefreshCfg* const RefreshManager::GetUserRefreshCfg(uint64_t nId)const
{
	auto it = m_mUserRefreshCfgs.find(nId);
	if (it == m_mUserRefreshCfgs.end())
	{
		return nullptr;
	}
	return &it->second;
}

UserRefreshCfg* RefreshManager::getUserRefreshCfg(uint64_t nId)
{
	auto it = m_mUserRefreshCfgs.find(nId);
	if (it == m_mUserRefreshCfgs.end())
	{
		return nullptr;
	}
	return &it->second;
}

bool RefreshManager::loadUserRefreshCfg()
{
	UserRefreshCfg cfg;
	//load from xml
	cfg.vRefreshTime.emplace_back(std::make_tuple(23, 0));
	cfg.nIntervalType = eIT_Seconds;
	cfg.nIntervalParam = 5;
	cfg.nRefreshModule = 0;

	//calc time
	if (!calcRefreshTime(&cfg))
	{
		MP_ERROR("Calculate Refresh Time Error! Id [%d]", cfg.nId);
		return false;
	}

	if (!m_mUserRefreshCfgs.emplace(cfg.nId, cfg).second)
	{
		MP_ERROR("User Refresh Same Id!", cfg.nId);
		return false;
	}
	return true;
}

bool RefreshManager::calcRefreshTime(UserRefreshCfg* pCfg)
{
	for (auto& time_cfg_info : pCfg->vRefreshTime)
	{
		meplay::MPTime refresh_time(std::get<0>(time_cfg_info), std::get<1>(time_cfg_info));
		uint32_t nOffset(0), nIntervalSec(0);
		if (!calcRefreshOffset(pCfg, nOffset, nIntervalSec))
		{
			return false;
		}

		if (nIntervalSec == 0)
		{
			return false;
		}
		pCfg->nIntervalSec = nIntervalSec;

		auto& now = GAME_CUR_TIME;
		if (refresh_time < now)
		{
			//计算下次刷新时间
			refresh_time += ((now.CurrentSec() - refresh_time.CurrentSec()) / nIntervalSec )* nIntervalSec;
		}
		else
		{
			//计算上次刷新时间
			refresh_time -= (((refresh_time.CurrentSec() - now.CurrentSec()) / nIntervalSec) + 1)* nIntervalSec;
		}

		pCfg->vCalcTime.emplace_back(
			std::make_tuple(
				refresh_time + nOffset - nIntervalSec,
				refresh_time + nOffset
			)
		);

#ifdef SSJ_DEBUG
		auto a1 = (refresh_time + nOffset - nIntervalSec) .CurrentSec();
		auto a2 = (refresh_time + nOffset).CurrentSec();
#endif // _SSJ_DEBUG
	}
	return true;
}

bool RefreshManager::calcRefreshOffset(UserRefreshCfg* pCfg, uint32_t& nOffset, uint32_t& nIntervalSec)
{
	switch (pCfg->nIntervalType)
	{
	case eIT_Seconds:
	{
		nOffset = pCfg->nIntervalParam;
		nIntervalSec = pCfg->nIntervalParam;
	}
	break;
	case eIT_Mins:
	{
		nOffset = pCfg->nIntervalParam * 60;
		nIntervalSec = pCfg->nIntervalParam * 60;
	}
	break;
	case eIT_Hours:
	{
		nOffset = pCfg->nIntervalParam * 3600;
		nIntervalSec = pCfg->nIntervalParam * 3600;
	}
	break;
	case eIT_Days:
	{
		nOffset = pCfg->nIntervalParam * 86400;
		nIntervalSec = pCfg->nIntervalParam * 86400;
	}
	break;
	case eIT_Special_Week_Day:
	{
		if (pCfg->nIntervalParam > 6)
		{
			return false;
		}

		auto& now = GAME_CUR_TIME;
		auto tmNow = now.ToTm();
		int nGap = (int)pCfg->nIntervalParam - (int)tmNow.tm_wday;
		nOffset = nGap > 0 ? nGap : 7 + nGap;
		nOffset *= 86400;
		nIntervalSec = 7 * 86400;
	}
	break;
	default:
		return false;
	}

	return true;
}

void RefreshManager::registerUserAllRefresh()
{
	for (auto& ci : m_mUserRefreshCfgs)
	{
		registerUserRefresh(&ci.second);
	}
}

void RefreshManager::registerUserRefresh(UserRefreshCfg* pCfg)
{
	auto& now = GAME_CUR_TIME;
	for (auto& time : pCfg->vCalcTime)
	{
		auto& tNextTime = std::get<1>(time);
#ifdef SSJ_DEBUG
		auto sPrev = std::get<0>(time).CurrentSec();
		auto sNext = tNextTime.CurrentSec();
		auto sNow = now.CurrentSec();
#endif
		if (tNextTime > now)
		{
			REGISTER_TIME_EVENT(
				eTET_SEC_INTERVAL, 
				pCfg->nIntervalSec, 
				tNextTime, 
				this, 
				&RefreshManager::GameUserOnlineRefresh,
				pCfg->nId
			);
		}
	}
}