#ifndef CONFIG_H
#define CONFIG_H

/* #define HASLOCK */
/* #define HASXRANDR */

#ifdef HASLOCK
#   define LOCK { "/usr/bin/xautolock", "-locknow", NULL }
#endif  /* LOCK */

#ifdef HASXRANDR
#   define XRANDR { "/bin/sh", "/usr/local/bin/xrandr.sh", NULL }
#endif  /* XRANDR */

#endif /* CONFIG_H */
