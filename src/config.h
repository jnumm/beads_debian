#ifndef _CONFIG_H
#define _CONFIG_H

#define BEADS_VERSION "1.1.13d"

#define cimg_OS 1

#if cimg_OS==2
//Windows
//#define cimg_use_magick
#define cimg_display_type  0

//#ifdef MINGW32
// in Cimg : Unknown configuration : ask for minimal dependencies (no display).
//#define cimg_OS            2
//#define cimg_display_type  0
//#endif

#endif

#endif /* _CONFIG_H */
