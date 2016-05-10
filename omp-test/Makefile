# -*-Mode: makefile;-*-

#*BeginPNNLCopyright*********************************************************
#
# $HeadURL$
# $Id$
#
#***********************************************************EndPNNLCopyright*

#****************************************************************************
# $HeadURL$
#
# Nathan Tallent
#****************************************************************************

CC = gcc -fopenmp

CFLAGS = -std=c99 -O3 -g

#****************************************************************************

omptest = omp-test

$(omptest)_SRCS = omp-test.c

$(omptest)_CFLAGS =

$(omptest)_LDFLAGS =

$(omptest)_LIBS =

$(omptest)_LDADD =


# omp-workshare1.c
# omp-workshare2.c
# omp-orphan.c
# omp-reduction.c


ompmm = omp-mm

$(ompmm)_SRCS = omp-mm.c

$(ompmm)_CFLAGS = #-DDO_SMALL

$(ompmm)_LDFLAGS =

$(ompmm)_LIBS =

$(ompmm)_LDADD =


#****************************************************************************

MK_SUBDIRS =

MK_PROGRAMS_C = $(omptest) $(ompmm)

include ../Makefile-template.mk

#****************************************************************************

