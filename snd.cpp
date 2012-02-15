#include "snd.h"
unsigned int SoundInitializer::initialized=0;

Snd::Snd()
{
	SoundInitializer::initialize();
}
Snd::Snd(const std::string& file)
{
	SoundInitializer::initialize();
	load(file);
}

Snd::~Snd()
{
	SoundInitializer::uninitialize();
}

void Snd::load(const std::string& file)
{
	handle=BASS_StreamCreateFile(0,file.c_str(),0,0,BASS_SAMPLE_LOOP);
	if(!handle) SoundInitializer::err("Error in file.");
}

void Snd::loadMOD(const std::string& file)
{
	if(!(handle=BASS_MusicLoad(0,file.c_str(),0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,0))) SoundInitializer::err("Error in file");
	BASS_ChannelSetAttribute(handle, BASS_ATTRIB_MUSIC_PSCALER, 1);
}

void Snd::play() const
{
	BASS_ChannelPlay(handle, 0);
}

void Snd::pause() const
{
	BASS_ChannelPause(handle);
}

std::pair<int,int> Snd::getMODPosition() const
{
	long pos=BASS_ChannelGetPosition(handle,BASS_POS_MUSIC_ORDER);
	return std::make_pair((pos&0xFFFF), (pos>>16));
}

unsigned long long Snd::getPosition() const
{
	return BASS_ChannelGetPosition(handle,BASS_POS_BYTE);
}

unsigned int Snd::getSeconds() const
{
	return BASS_ChannelBytes2Seconds(handle,getPosition());
}

#ifdef _WIN32
	void __stdcall Snd::SyncInit(HSYNC handle, DWORD channel, DWORD data, void* user)
	{
		SYNCPROC asd;
		SyncData* d = (SyncData*)user;
		d->syncfunc(d->args);
	}
#else
	void Snd::SyncInit(uint32_t handle, uint32_t channel, uint32_t data, void* user)
	{
		SyncData* d = (SyncData*)user;
		d->syncfunc(d->args);
	}
#endif

void Snd::syncPosition(t_SyncFunc f, unsigned short order, unsigned short row, void* args)
{
	uint32_t type=((row<<16)|order);
	data = SyncData(f, args);
	BASS_ChannelSetSync(handle, BASS_SYNC_MUSICPOS, type, SyncInit, &data);
}
/*
int main(int argc, char** argv)
{
	Snd s;
	s.loadMOD(argv[1]);
	s.play();
	for(int i=0;i<100000; ++i)
	{
		std::pair<int,int> pos = s.getMODPosition();
		//std::cout << pos.first << " - " << pos.second << std::endl;
		if((pos.second%16)==0) std::cout << "bam" << std::endl;
		usleep(10000);
	}
	return 0;
}
*/
