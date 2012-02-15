#pragma once
#include <iostream>
#include <stdint.h>
#include <sstream>
#include "bass.h"

class SoundInitializer
{
	private:
		static unsigned int initialized;
		SoundInitializer() {}
	public:
		static void initialize()
		{
			if(!initialized) if(!BASS_Init(-1,44100,0,0,NULL)) err("Cannot init audio device");
			initialized++;
		}
		static void uninitialize()
		{
			if(initialized) initialized--;
			if(!initialized) BASS_Free();
		}
		static void err(const std::string& text)
		{
			int err=BASS_ErrorGetCode();
			std::stringstream ss;
			ss<<err;
			std::string errstr;
			ss>>errstr;
			std::cout << "Error(" + text + "): " + errstr << std::endl;
			BASS_Free();
		}
};

class Snd
{
	private:
		typedef void(*t_SyncFunc)(void*);
		int handle;
		void err(const std::string& msg);

		#ifdef _WIN32
			static void __stdcall SyncInit(HSYNC handle, DWORD channel, DWORD data, void* user);
		#else
			static void SyncInit(uint32_t, uint32_t, uint32_t, void*);
		#endif
		struct SyncData
		{
			SyncData() : syncfunc(NULL), args(NULL) {}
			SyncData(t_SyncFunc f, void* a) : syncfunc(f), args(a) {}
			t_SyncFunc 	syncfunc;
			void*		args;
		};

		SyncData data;
	public:
		Snd();
		Snd(const std::string& file);
		~Snd();
		void load(const std::string& file);
		void loadMOD(const std::string& file);
		void play() const;
		void pause() const;
		unsigned long long getPosition() const;
		std::pair<int,int> getMODPosition() const;
		unsigned int getSeconds() const;
		bool get4th() const;
		bool get8th() const;
		bool get16th() const;
		bool getMeasure() const;
		bool getCustom(unsigned int position) const;
		void syncPosition(t_SyncFunc f, unsigned short order, unsigned short row, void* args);
};
