#include "stdafx.h"
#include <stdlib.h>
#include <vlc/vlc.h>
#include <Windows.h>
int main(int argc, char* argv[])
{
	libvlc_instance_t * inst;
	libvlc_media_player_t *mp;
	libvlc_media_t *m;


	//char *myarg0 = "--sout=#transcode{vcodec=h264,scale=Auto,acodec=mpga,ab=128,channels=2,samplerate=44100}:duplicate{dst=display,dst=rtp{sdp=rtsp://destinationIP:Port}}";
	char *myarg1 = "--rtsp-tcp";
	char *myargs[2] = { myarg1, /*myarg0*/ };
	/* Load the VLC engine */
	inst = libvlc_new(1, myargs);

	/* Create a new item */
	char *input = "rtsp://root:e9K4Hf0EBp1t@147.87.113.151/axis-media/media.amp?videocodec=h264&resolution=1280x800";
	m = libvlc_media_new_location(inst, input);

	/* Create a media player playing environement */
	mp = libvlc_media_player_new_from_media(m);

	/* No need to keep the media now */
	libvlc_media_release(m);

	/* play the media_player */
	libvlc_media_player_play(mp);

	Sleep(10000000); /* Let it play a bit */

	libvlc_media_player_stop(mp); /* Stop playing */

	libvlc_media_player_release(mp); /* Free the media_player */

	libvlc_release(inst);

	return 0;
}