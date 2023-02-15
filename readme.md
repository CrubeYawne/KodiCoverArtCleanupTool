# Kodi Cover Art Cleanup Tool

This is a command line tool used to cleanup exported NFO files used by KODI. 

## Reason:
When Kodi scrapes a movie it stores a copy of the cover art with the URL of the art itself. When you export your library it dumps this URL regardless of the endpoint status. Since most of the cover art is not stored or servered publically on a permanent basis this leads to dead information when re-importing a library.

## This tool can do the following:
1. Remove cover art URLS that are dead (404)
2. Remove NFO files that contain no healthy cover URLs

**NOTE: There is a 500ms delay between calls to avoid bombarding public servers**

The program will execute by default in a "safe" mode and make no changes to the target files.

Use the included parameters to act on target files.

## Example

KodiCoverArtCleanupTool.exe D:\Movies\

*Will scan directory and make no changes*

KodiCoverArtCleanupTool.exe D:\Movies\ -d

*Will scan directory and only remove files without any working cover art*

KodiCoverArtCleanupTool.exe D:\Movies\ -e

*Will scan directory and will remove cover art from files that are invalid(404)*

KodiCoverArtCleanupTool.exe D:\Movies\ -e -d

*Will scan directory and will remove cover art from files that are invalid(404) and remove files without any working cover art*


## Sample Console output
```
Checking target directory [D:\Movies\]
Found 1438 items


Loop: 0/1437
	Checking file: D:/Movie.nfo
	Contains thumbnails
	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/4BVqrvv7juGRZJd2XdHevOFTBEo.jpg">http://image.tmdb.org/t/p/original/4BVqrvv7juGRZJd2XdHevOFTBEo.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/4BVqrvv7juGRZJd2XdHevOFTBEo.jpg] 66
	HTTP CODE: 200
	Has good thumbnail

Loop: 75/1437
	Checking file: D:/Movie3.nfo
	No thumbnails

Loop: 109/1437
	Checking file: D:/Movie2.nfo
	Contains thumbnails
	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/58epoWcaKK4dXRMWUpbcPdFzG3s.jpg">http://image.tmdb.org/t/p/original/58epoWcaKK4dXRMWUpbcPdFzG3s.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/58epoWcaKK4dXRMWUpbcPdFzG3s.jpg] 66
	HTTP CODE: 404

	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/b7rNEFf4aSMVY5eL5VIbpmjchrL.jpg">http://image.tmdb.org/t/p/original/b7rNEFf4aSMVY5eL5VIbpmjchrL.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/b7rNEFf4aSMVY5eL5VIbpmjchrL.jpg] 66
	HTTP CODE: 404

	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/5iara9JMotSn9r7TM4tL7LFyfdu.jpg">http://image.tmdb.org/t/p/original/5iara9JMotSn9r7TM4tL7LFyfdu.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/5iara9JMotSn9r7TM4tL7LFyfdu.jpg] 66
	HTTP CODE: 404

	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/jlMffQe3o6X4hTzhujEthOsge7u.jpg">http://image.tmdb.org/t/p/original/jlMffQe3o6X4hTzhujEthOsge7u.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/jlMffQe3o6X4hTzhujEthOsge7u.jpg] 66
	HTTP CODE: 404

	Checking <thumb aspect="poster" preview="http://image.tmdb.org/t/p/w500/s0HWM0TZ1eKO9prRgz5gF0YGGps.jpg">http://image.tmdb.org/t/p/original/s0HWM0TZ1eKO9prRgz5gF0YGGps.jpg</thumb>
	Cleaned: [http://image.tmdb.org/t/p/original/s0HWM0TZ1eKO9prRgz5gF0YGGps.jpg] 66
	HTTP CODE: 200
	Has good thumbnail

REMOVING:
	D:/Movie4.nfo
	D:/Movie5.nfo
	D:/Movie6.nfo
```	


