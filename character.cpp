#include <stdafx.h>
#include "character.h"
#include <tinyxml.h>

#include <params.h>
#include <steering/seekSteering.h>
#include <steering/arriveSteering.h>
#include <steering/alignToMovementSteering.h>
#include <steering/alignSteering.h>


const float Character::MIN_SPEED_TO_CHANGE_DESTINATION = 10.f;

Character::Character() : 
	mLinearVelocity  (0.0f, 0.0f), 
	mAngularVelocity (0.0f),
	mSteering        (nullptr),
	mAlignSteering   (nullptr),
	mDestination     (0.f, 0.f)
{
	RTTI_BEGIN
		RTTI_EXTEND (MOAIEntity2D)
	RTTI_END
}

Character::~Character()
{
	if (mSteering)      delete mSteering;
	if (mAlignSteering) delete mAlignSteering;
}

void Character::OnStart()
{
    ReadParams("params5.xml", mParams);
	//mSteering = new SeekSteering(*this, mParams.targetPosition);
	mDestination = mParams.targetPosition;
	mSteering = new ArriveSteering(*this, mDestination);
	//mAlignSteering = new AlignSteering(*this, mParams.targetRotation);
	mAlignSteering = new AlignToMovementSteering(*this);
}

void Character::OnStop()
{

}

void Character::OnUpdate(float step)
{
	USVec3D pos             = GetLoc();
	float   rot             = GetRot();

	pos.mX += mLinearVelocity.mX * step;
	pos.mY += mLinearVelocity.mY * step;

	rot += mAngularVelocity * step;

	SetLoc(pos);
	SetRot(rot);

	if (mSteering) {
		USVec2D linearAcceleration = mSteering->GetSteering();
		mLinearVelocity.mX += linearAcceleration.mX * step;
		mLinearVelocity.mY += linearAcceleration.mY * step;

		if (mLinearVelocity.LengthSquared() < MIN_SPEED_TO_CHANGE_DESTINATION * MIN_SPEED_TO_CHANGE_DESTINATION) {
			mDestination.mX = Rand(-512, 512);
			mDestination.mY = Rand(-384, 384);
			mSteering->SetTargetPosition(mDestination);
		}
	}

	if (mAlignSteering) {
		float angularAcceleration = mAlignSteering->GetSteering();
		mAngularVelocity += angularAcceleration * step;
	}
}

void Character::DrawDebug()
{
	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get();

	/*gfxDevice.SetPenColor(0.5f, 0.5f, 1.0f, 0.5f);
	MOAIDraw::DrawEllipseOutline(mParams.targetPosition.mX, mParams.targetPosition.mY, mParams.arrive_radius, mParams.arrive_radius, 50);

	gfxDevice.SetPenColor(0.5f, 1.0f, 0.5f, 0.5f);
	MOAIDraw::DrawPoint(mParams.targetPosition);
	MOAIDraw::DrawEllipseOutline(mParams.targetPosition.mX, mParams.targetPosition.mY, mParams.dest_radius, mParams.dest_radius, 50);*/
	
	gfxDevice.SetPenColor(0.0f, 1.0f, 0.5f, 0.5f);
	MOAIDraw::DrawPoint(mDestination);
	MOAIDraw::DrawEllipseOutline(mDestination.mX, mDestination.mY, 5.f, 5.f, 50);


	if (mSteering)      mSteering->DrawDebug();
	if (mAlignSteering) mAlignSteering->DrawDebug();
}





// Lua configuration

void Character::RegisterLuaFuncs(MOAILuaState& state)
{
	MOAIEntity2D::RegisterLuaFuncs(state);
	
	luaL_Reg regTable [] = {
		{ "setLinearVel",			_setLinearVel},
		{ "setAngularVel",			_setAngularVel},
		{ NULL, NULL }
	};

	luaL_register(state, 0, regTable);
}

int Character::_setLinearVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float pX = state.GetValue<float>(2, 0.0f);
	float pY = state.GetValue<float>(3, 0.0f);
	self->SetLinearVelocity(pX, pY);
	return 0;	
}

int Character::_setAngularVel(lua_State* L)
{
	MOAI_LUA_SETUP(Character, "U")
	
	float angle = state.GetValue<float>(2, 0.0f);
	self->SetAngularVelocity(angle);

	return 0;
}
	