#
# Makefile
# Written in 2022 by Márcio Silva <coadde@hyperbola.info>
#
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.
#
# You should have received a copy of the CC0 Public Domain Dedication along
# with this software. If not, see
# <https://creativecommons.org/publicdomain/zero/1.0/>.
#

BINDIR = usr/local/bin
CFLAGS = -std=c17
EXEC = gplgen

all: $(EXEC)_c

$(EXEC)_c: $(EXEC).c
	$(CC) $(CFLAGS) -o $@ $<

install: $(EXEC)_c $(EXEC).py $(EXEC).php $(EXEC).lua $(EXEC).sh
	for i in $?; do \
            export j=$$(printf '%s\n' $$i | sed 's/[.]/_/'); \
	    install -Dm 0755 $$i $(DESTDIR)/$(BINDIR)/$$j; \
	done
	unset i j

cleanall: _PHONY
	$(RM) $(EXEC)_c

_PHONY:
