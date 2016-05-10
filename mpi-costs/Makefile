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

CXX = mpicxx
CC  = mpicc

CXXFLAGS = -O1 -g # avoid unsafe Intel optimizations
CFLAGS   = -O3 -g

ifeq ($(MYCFG_DOMAINNAME),pic.pnnl.gov)
  CPUINFO = -DMY_CPU_MHZ=2100 # PIC/pal
endif
ifeq ($(MYCFG_DOMAINNAME),cluster.pnnl.gov)
  CPUINFO = -DMY_CPU_MHZ=2800 # SeaPearl
endif

#DO_PAPI = 1
PAPI = $(HOME)/pkg/papi
PAPI_LIB = $(PAPI)/lib
PAPI_INC = $(PAPI)/include

#****************************************************************************

mpicosts = mpi-costs

$(mpicosts)_SRCS = main.cpp timer.c

$(mpicosts)_CFLAGS = $(CPUINFO)
$(mpicosts)_CXXFLAGS = $(CPUINFO)
ifdef DO_PAPI
  $(mpicosts)_CXXFLAGS += -I$(PAPI_INC)
endif

$(mpicosts)_LDFLAGS =

$(mpicosts)_LIBS =

$(mpicosts)_LDADD =
ifdef DO_PAPI
  $(mpicosts)_LDADD += -L$(PAPI_LIB) -Wl,-rpath $(PAPI_LIB) -lpapi
endif
$(mpicosts)_LDADD += -lrt # clock_gettime

#****************************************************************************

MK_SUBDIRS =

MK_PROGRAMS_CXX = $(mpicosts)

include ../Makefile-template.mk

#****************************************************************************
