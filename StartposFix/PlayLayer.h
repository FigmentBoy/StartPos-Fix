#pragma once
#include "Checkpoint.h"
#include "MinHook.h"

namespace PlayLayer {
	inline bool(__thiscall* init)(void* self, void* GJGameLevel);
	bool __fastcall initHook(void* self, void*, void* GJGameLevel);

	inline void(__thiscall* togglePractice)(void* self, bool practice);
	void __fastcall hkTogglePractice(void* self, int edx, bool practice);

	inline int(__thiscall* resetLevel)(void* self);
	int __fastcall hkResetLevel(void* self);

	inline void(__thiscall* update)(void* self, float delta);
	void __fastcall hkUpdate(void* self, void*, float delta);

	void mem_init();

	extern CheckPoint checkpoint;
	extern bool inPractice;
	extern bool inTestmode;
	extern int smoothOut;
}

namespace CCDirector {
	inline void* (__cdecl* sharedState)();
	inline double (__thiscall* getAnimationInterval)(void*);
}