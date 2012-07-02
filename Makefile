TARGET = boxsuspend
SOURCES = $(wildcard *.c)
OBJECTS = $(subst .c$,.o,$(SOURCES))
HEADERS = $(wildcard *.h)
CC = gcc
LINK = gcc
CFLAGS = -ansi -pedantic
LFLAGS = 
STRIP = strip
INSTALL = install
INSTALL_ARGS = -o root -g wheel -m 4755  # Installs with SUID set
INSTALL_DIR = /usr/local/bin/

# Autoconfiguration
SUSPENDFILE="/sys/power/state"

all: debug

debug: CFLAGS += -g -DDEBUG
debug: LFLAGS += -g
debug: build

release: CFLAGS += -Os
release: LFLAGS += 
release: clean build
	$(STRIP) $(TARGET)

build: build_host.h $(TARGET)

$(TARGET): build_host.h $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBPATH) $(LIBS)

.c.o: $*.h common.h
	$(CC) -c $(CFLAGS) $(DEBUGFLAGS) $(INCPATH) -o $@ $<

# now the program gets autoconfigured in Makefile via build_host.h
build_host.h:
	@echo -n "Generating configuration: "
	@echo "#define BUILD_HOST \"`hostname`\"" > build_host.h;
	@echo "#define BUILD_KERNEL \"`uname -rm`\"" >> build_host.h;
	@echo "#define SUSPENDFILE \"${SUSPENDFILE}\"" >> build_host.h
	@echo DONE

install: release
	$(INSTALL) $(INSTALL_ARGS) $(TARGET) $(INSTALL_DIR)
	@echo "DONE"

uninstall:
	-rm -f $(INSTALL_DIR)$(TARGET)
	@echo "DONE"

clean:
	-rm -f .depend
	-rm -f $(OBJECTS)
	-rm -f *~ core *.core
	-rm -f version.h
	-rm -f $(TARGET)
	-rm -f build_host.h

.PHONY : all debug release build clean install uninstall
