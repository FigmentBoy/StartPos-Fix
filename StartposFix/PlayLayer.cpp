#include <cocos2d.h>
#include "PlayLayer.h"
#include <vector>

void WriteBytes(void* location, std::vector<BYTE> bytes) {
	DWORD old_prot;
	VirtualProtect(location, bytes.size(), PAGE_EXECUTE_READWRITE, &old_prot);

	memcpy(location, bytes.data(), bytes.size());

	VirtualProtect(location, bytes.size(), old_prot, &old_prot);
}

namespace PlayLayer {
	bool inPractice;
	bool inTestmode;
	int smoothOut;

	bool __fastcall initHook(void* self, void*, void* GJGameLevel) {
		size_t base = (size_t)GetModuleHandle(0);

		bool res = init(self, GJGameLevel);
		inPractice = false;
		inTestmode = *(bool*)((uintptr_t)self + 0x494);
		smoothOut = 0;


		return res;
	}

	void __fastcall hkTogglePractice(void* self, int edx, bool practice) {
		size_t base = (size_t)GetModuleHandle(0);

		inPractice = practice;

		return togglePractice(self, practice);
	}

	int __fastcall hkResetLevel(void* self) {
		if (inTestmode || inPractice) {
			smoothOut = 2; // Account for 1 extra frame respawn
		}
		return resetLevel(self);
	}

	void __fastcall hkUpdate(void* self, void*, float delta) {

		if (!smoothOut) {
			return update(self, delta);
		}

		float time = cocos2d::CCDirector::sharedDirector()->getAnimationInterval();
		if (smoothOut != 0 && delta - time < 1) { // if close enough to normal speed
			smoothOut --;
		}

		return update(self, time);

	}

	void mem_init() {
		size_t base = (size_t)GetModuleHandle(0);

		MH_CreateHook(
			(PVOID)(base + 0x01FB780),
			initHook,
			(LPVOID*)&init);

		MH_CreateHook(
			(PVOID)(base + 0x20BF00),
			hkResetLevel,
			(LPVOID*)&resetLevel);

		MH_CreateHook(
			(PVOID)(base + 0x20D0D0),
			hkTogglePractice,
			(LPVOID*)&togglePractice);

		MH_CreateHook(
			(PVOID)(base + 0x2029C0),
			PlayLayer::hkUpdate,
			(LPVOID*)&PlayLayer::update);

	}
}
