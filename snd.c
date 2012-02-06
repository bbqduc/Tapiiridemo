#include "musicmanager.h"

void err(const char* text)
{
	fprintf(stderr,"Error(%d): %s\n", BASS_ErrorGetCode(), text);
	BASS_Free();
	exit(EXIT_FAILURE);
}

void snd_init()
{
	if(!BASS_Init(-1,44100,0,0,NULL)) err("Cannot init audio device");
}
void snd_destroy()
{
	BASS_Free();
}

int snd_loadMOD(const char* file)
{
	int chan;
	if(!(chan=BASS_MusicLoad(0,file,0,0,BASS_SAMPLE_LOOP|BASS_MUSIC_RAMPS|BASS_MUSIC_PRESCAN,0))) err("Error in file");
	return chan;
}

int snd_load(const char* file)
{
	int chan;
	if(!(chan=BASS_StreamCreateFile(0,file,0,0,BASS_SAMPLE_LOOP))) err("Error in file.");
	return chan;
}

void snd_play(int chan)
{
	BASS_ChannelPlay(chan, 0);
}

unsigned int snd_getPosition(int chan)
{
	return BASS_ChannelGetPosition(chan,BASS_POS_BYTE);
}
unsigned int snd_getSeconds(int chan)
{
	return BASS_ChannelBytes2Seconds(chan,snd_getPosition(chan));
}

/*
int main(int argc, char** argv)
{
	snd_init();
	int handle=snd_load(argv[1]);
	snd_play(handle);
	for(int i=0;i<5;++i) { sleep(1); printf("Pos: %u, Time: %u\n", snd_getPosition(handle), snd_getSeconds(handle)); }
	snd_destroy();
}
*/
