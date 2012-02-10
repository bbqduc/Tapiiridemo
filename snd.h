#pragma once
#include <iostream>
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

		static void SyncInit(int, int, int, void*);
		struct SyncData
		{
			SyncData() : m_Func(NULL), m_Arg(NULL) {}
			SyncData(t_SyncFunc f, void* a) : m_Func(f), m_Arg(a) {}
			t_SyncFunc 	m_Func;
			void*		m_Arg;
		};
		SyncData data;
	public:
		Snd();
		Snd(const std::string& file);
		~Snd();
		void load(const std::string& file);
		void loadMOD(const std::string& file);
		void play() const;
		unsigned long long getPosition() const;
		std::pair<int,int> getMODPosition() const;
		unsigned int getSeconds() const;
		bool get4th() const;
		bool get8th() const;
		bool get16th() const;
		bool getMeasure() const;
		bool getCustom(unsigned int position) const;
};
