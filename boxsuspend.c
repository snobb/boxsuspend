/**
 *      boxsuspend.c
 *
 *      Copyright 2011 Alex Kozadaev <akozadaev at yahoo dot com>
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following disclaimer
 *        in the documentation and/or other materials provided with the
 *        distribution.
 *      * Neither the name of the  nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "build_host.h"

#define NAME    "boxsuspend"
#define VERSION "0.11"
#define MEM     "mem"
#define DISK    "disk"
#define error(msg) { printf("ERROR: %s\n",msg); exit(1);}

#define PARAMS { "/usr/bin/xautolock", "-locknow", NULL }
#define XRANDR { "/bin/sh", "/usr/local/bin/xrandr.sh", NULL }


void usage();
void spawn(const char **);

int
main(int argc, char *argv[])
{
	FILE *output = NULL;
	char value[4] = MEM;
	const char *params[] = PARAMS;
	const char *xrandr[] = XRANDR;

#ifndef DEBUG
	if (geteuid()) {
		usage();
		error("Must be root");
		return 1;
	}
#endif

	if (argc > 1) {
		if (*argv[1]++ == '-') {
			switch (*argv[1]) {
			case 'd': strcpy(value, DISK); break;
			case 'h': usage(); exit(0); break;
			}
		} 
	} 

#ifndef DEBUG
	spawn(params);
	sleep(1);

	output = fopen(SUSPENDFILE, "w");
	if (output == NULL)
		error("Cannot open kernel pipe");

	fprintf(output, "%s", value);
	fclose(output);

	sleep(5);
	spawn(xrandr);
#else
	printf("Current value: %s\n", value);
#endif

	return 0;
}

void
spawn(const char **params) {
	if (fork() == 0) {
		setsid();
		execv(params[0], (char**)params);
		exit(0);
	}
}

void
usage()
{
	puts(NAME " v" VERSION " Alex Kozadaev (c) [" BUILD_KERNEL "]\n");
	puts("\t" NAME " [-d] [-h]\n");
	puts("\t-d - hybernate (default - suspend to memory)");
	puts("\t-h - help (this output)");
	puts("\n\tNOTE: works only with 2.6+ linux kernels\n");
}
