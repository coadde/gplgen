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

all: _PHONY
	${MAKE} -C src

install: _PHONY
	${MAKE} -C src install

cleanall: _PHONY
	${MAKE} -C src cleanall

_PHONY: