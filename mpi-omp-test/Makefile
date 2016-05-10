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

CC = mpicc -fopenmp

CFLAGS = -std=c99 -O3 -g

#****************************************************************************

mpiomptest = mpi-omp-test

$(mpiomptest)_SRCS = mpi-omp-test.c

$(mpiomptest)_CFLAGS =

$(mpiomptest)_LDFLAGS =

$(mpiomptest)_LIBS =

$(mpiomptest)_LDADD =


mpiompmm = mpi-omp-mm

$(mpiompmm)_SRCS = mpi-omp-mm.c

$(mpiompmm)_CFLAGS = #-DDO_SMALL

$(mpiompmm)_LDFLAGS =

$(mpiompmm)_LIBS =

$(mpiompmm)_LDADD =


#****************************************************************************

MK_SUBDIRS =

MK_PROGRAMS_C = $(mpiomptest) $(mpiompmm)

include ../Makefile-template.mk

#****************************************************************************

