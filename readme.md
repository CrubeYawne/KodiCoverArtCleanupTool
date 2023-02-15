# Kodi Cover Art Cleanup Tool

This is a command line tool used to cleanup exported NFO files used by KODI. 

Reason:
When Kodi scrapes a movie it stores a copy of the cover art with the URL of the art itself. When you export your library it dumps this URL regardless of the endpoint status. Since most of the cover art is not stored or servered publically on a permanent basis this leads to dead information when re-importing a library.

This tool can do the following:
1. Remove cover art URLS that are dead (404)
2. Remove NFO files that contain no healthy cover URLs

**NOTE: There is a 500ms delay between calls to avoid bombarding public servers**

The program will execute by default in a "safe" mode and make no changes to the target files.

Use the included parameters to act on target files.