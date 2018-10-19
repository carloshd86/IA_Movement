#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <gameEntity.h>
#include <steering/iSteering.h>
#include <steering/iAlignSteering.h>

class Enemy: public GameEntity
{
public:
    DECL_LUA_FACTORY(Enemy)
protected:
	virtual void OnStart();
	virtual void OnStop();
	virtual void OnUpdate(float step);
public:
	virtual void DrawDebug();

	Enemy();
	~Enemy();
	
	void SetTarget(GameEntity& target);
	void SetLinearVelocity(float x, float y) { mLinearVelocity.mX = x; mLinearVelocity.mY = y;}
	void SetAngularVelocity(float angle) { mAngularVelocity = angle;}
	
	USVec2D GetLinearVelocity() const { return mLinearVelocity;}
	float GetAngularVelocity() const { return mAngularVelocity;}

	const Params& GetParams() const { return mParams; }

private:

	static const float MIN_DISTANCE_TO_REACH_TARGET;

	USVec2D mLinearVelocity;
	float mAngularVelocity;
	
	Params mParams;

	ISteering      * mSteering;
	IAlignSteering * mAlignSteering;
	GameEntity     * mTarget;

	// Lua configuration
public:
	virtual void RegisterLuaFuncs(MOAILuaState& state);
private:
	static int _setLinearVel(lua_State* L);
	static int _setAngularVel(lua_State* L);
	static int _setTarget(lua_State* L);
};

#endif