#
# Copyright (C) 2014. See LICENSE in top-level directory.
#

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/src

lib_LTLIBRARIES = libkahanmpi.la

noinst_LTLIBRARIES = libkahanmpii.la

libkahanmpi_la_SOURCES = src/coll.c \
                         src/initthread.c \
                         src/gather_and_compensated.c \
                         src/userdef_compensated.c \
                         src/promote.c \
                         src/utils.c

libkahanmpi_la_LDFLAGS = -version-info $(libkahanmpi_abi_version)

libkahanmpii_la_SOURCES = $(libkahanmpi_la_SOURCES)
libkahanmpii_la_LDFLAGS = $(libkahanmpi_abi_version)

include_HEADERS = src/kahanmpi.h
noinst_HEADERS = src/mybool.h src/myquad.h src/state.h src/utils.h

bin_PROGRAMS =
check_PROGRAMS = 
TESTS = 
XFAIL_TESTS = 

MPIEXEC = mpiexec -n 2
TESTS_ENVIRONMENT = $(MPIEXEC)

include test/Makefile.mk

.PHONY: checkprogs
checkprogs: $(check_PROGRAMS)
