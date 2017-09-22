#include "stdafx.h"
#include <vlc/vlc.h>
#include <string>
#include <iostream>

using namespace std;

static int frameCount = 1;

char* buf = NULL;

static void* lock(void* userData, void** p_pixels) {
	char* buffer = (char*)userData;
	*p_pixels = buffer;
	cout << "lock no: " << frameCount << endl;
	return NULL;
}

static void unlock(void* userData, void* picture, void *const * p_pixels) {
	cout << "unlock no: " << frameCount++ << endl;
}

int main4(int argc, char *argv[])
{

	//string fname("path/to/my/movie.mov");
	char *fname = "rtsp://root:e9K4Hf0EBp1t@147.87.113.151/axis-media/media.amp?videocodec=h264&resolution=1280x800&duration=10&clock=1";

	libvlc_instance_t* vlcInstance = libvlc_new(0, NULL);
	libvlc_media_t* media = libvlc_media_new_location(vlcInstance, fname);
	libvlc_media_player_t* mediaPlayer = libvlc_media_player_new_from_media(media);
	libvlc_media_release(media);

	int wd = 1280, ht = 800;
	buf = new char[wd*ht * 4];

	libvlc_video_set_callbacks(mediaPlayer, lock, unlock, NULL, buf);
	// Should use libvlc_video_set_format_callbacks...
	libvlc_video_set_format(mediaPlayer, "RGBA", wd, ht, 4 * wd);

	libvlc_media_player_play(mediaPlayer);

	while (true) {
		;
	}
	return 0;
}
