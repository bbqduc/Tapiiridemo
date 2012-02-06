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
}

void Snd::play() const
{
	BASS_ChannelPlay(handle, 0);
}

unsigned long long Snd::getPosition() const
{
	return BASS_ChannelGetPosition(handle,BASS_POS_BYTE);
}

unsigned int Snd::getSeconds() const
{
	return BASS_ChannelBytes2Seconds(handle,getPosition());
}

int main(int argc, char** argv)
{
	Snd s(argv[1]);
	s.play();
	for(int i=0;i<5; ++i)
	{
		std::cout << s.getPosition() << " - " << s.getSeconds() << std::endl;
		sleep(1);
	}
	return 0;
}
