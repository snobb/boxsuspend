//      boxsuspend.c
//
//      Copyright 2011 Alex Kozadaev <akozadaev at yahoo dot com>
//
//      Redistribution and use in source and binary forms, with or without
//      modification, are permitted provided that the following conditions are
//      met:
//
//      * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following disclaimer
//        in the documentation and/or other materials provided with the
//        distribution.
//      * Neither the name of the  nor the names of its
//        contributors may be used to endorse or promote products derived from
//        this software without specific prior written permission.
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//      LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//      A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//      OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "build_host.h"

#define NAME		"boxsuspend"
#define VERSION	"0.1"
#define MEM			"mem"
#define DISK		"disk"
#define error(msg) { printf("ERROR: %s\n",msg); exit(1);}

#define PARAMS { "/usr/bin/xautolock", "-locknow", NULL }


void usage();
void locknow(const char **);

int
main(int argc, char *argv[])
{
	char value[4] = MEM;

	if (geteuid()) {
		usage();
		error("Must be root");
		return 1;
	}

	if (argc > 1) {
		if (*argv[1]++ == '-') {
			switch (*argv[1]) {
				case 'd': strncpy(value, DISK, sizeof(DISK)); break;
				case 'h': usage(); exit(0); break;
			}
		} 
	}	

	const char *params[] = PARAMS;
	locknow((const char **)params);
	sleep(1);

	FILE* output = fopen(SUSPENDFILE, "w");
	if (output == NULL)
		error("Cannot open kernel pipe");
#ifndef VERBOSE
	fprintf(output, "%s", value);
#endif
#ifdef VERBOSE
	printf("Current value: %s\n", value);
#endif
	fclose(output);
	return 0;
}

void
locknow(const char **params) {
	if (fork() == 0) {
		setsid();
		execv(params[0], (char**)params);
		exit(0);
	}
}

void
usage()
{
	printf("%s v%s (works only with 2.6+ kernels). Alex Kozadaev (c)\n", NAME, VERSION);
	printf("\t%s [-d]\n", NAME);
	printf("\t-d - hybernate (default - suspend to memory)\n");
}
