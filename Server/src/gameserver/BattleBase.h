#pragma once

class BattleBase
{
public:
	BattleBase() {};
	virtual ~BattleBase() {};
public:
	virtual bool BattleInit() = 0;
	virtual bool BattleStart() = 0;
	virtual bool BattleRun() = 0;
	virtual bool BattleEnd() = 0;
private:
};

class Battle
{

};