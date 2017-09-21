#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <vlc/vlc.h>
#include <windows.h>

int main2(int argc, char* argv[])
{
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;

	const char * const vlc_args[] = {
		//"--verbose=2",
		"--rtsp-tcp",
		"--network-caching=100",
	};

	/* Load the VLC engine */
	inst = libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);

	/* Create a new item */
	//m = libvlc_media_new_location(inst, "http://mycool.movie.com/test.mov");
	//m = libvlc_media_new_path (inst, "C:\\Users\\laupm4\\Desktop\\test.mp4");
	m = libvlc_media_new_path(inst, "rtsp://root:e9K4Hf0EBp1t@147.87.113.151/axis-media/media.amp?videocodec=h264&resolution=1280x800");

	/* Create a media player playing environement */
	mp = libvlc_media_player_new(inst);
	//mp = libvlc_media_player_new_from_media(m);

	libvlc_media_player_set_media(mp, m);
	

	/* No need to keep the media now */
	libvlc_media_release(m);

#if 0
	/* This is a non working code that show how to hooks into a window,
	* if we have a window around */
	libvlc_media_player_set_xwindow(mp, xid);
	/* or on windows */
	libvlc_media_player_set_hwnd(mp, hwnd);
	/* or on mac os */
	libvlc_media_player_set_nsobject(mp, view);
#endif

	/* play the media_player */
	libvlc_media_player_play(mp);

	Sleep(10000); /* Let it play a bit */

			   /* Stop playing */
	libvlc_media_player_stop(mp);

	/* Free the media_player */
	libvlc_media_player_release(mp);

	libvlc_release(inst);

	return 0;
}