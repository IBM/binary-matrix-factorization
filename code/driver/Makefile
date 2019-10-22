# Copyright (C) 2019, International Business Machines
# Corporation.  All Rights Reserved.

# This program is distributed under the terms of the
# Eclipse Public License - v 2.0

# Set the optimization level
#	OPT = -g
	OPT = -O

###############################################################################

# Set the compiler
CC = gcc
CXX = g++

# Set the working directory
ROOT = $(PWD)

# Paths
INCDIR = -I$(ROOT)/include 
//BINDIR = $(ROOT)/bin
BINDIR = $(ROOT)
SRCDIR = $(ROOT) 

# Flags
MORECFLAGS =
MORECFLAGS += -Wall 
#MORECFLAGS += -O3 -fomit-frame-pointer -pipe -DNDEBUG -pedantic-errors -Wimplicit -Wparentheses -Wreturn-type -Wcast-qual -Wall -Wpointer-arith -Wwrite-strings -Wconversion -Wno-unknown-pragmas  

##############################################################################
# Set the optimization level
# NOTE: if it is set to "-g" then everything is compiled debugging enabled.
#       if it is set to "-O" then it'll be reset to the highest possible opt
#       otherwise it'll left alone 
##############################################################################

ifeq ($(OPT),-O)
        OPT = -O6
endif

CFLAGS = $(OPT) $(MORECFLAGS) $(INCDIR)
CXXFLAGS = $(CFLAGS)

###############################################################################

# Source files
SRCFILES = driver.cpp
OBJFILES = $(SRCFILES:.cpp=.o)
DEPFILES = $(SRCFILES:.cpp=.d)

###############################################################################

#Rules

# NOTE: the '@' supresses the echoing of the command, and only its result 
#       will be seen

%.o : %.cpp
	@echo Compiling $*.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

%.d : %.cpp
	@echo Creating dependency $*.d from $*.cpp
	@echo -n $(dir $<) > $@
	@$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $< >> $@'

# What to make
default: $(BINDIR)/driver
driver: $(BINDIR)/driver

$(BINDIR)/driver : $(DEPFILES) $(OBJFILES)
	@echo ""
	@echo "Linking $(notdir $@) ..."
	@echo ""
	@mkdir -p $(BINDIR)
	@$(CXX) $(CFLAGS) -o $@ $(OBJFILES) $(LIBS)

# Special targets

.PHONY: clean
clean:
	rm -f *.o *.d driver

clean_all: clean
#	rm -rf $(BINDIR)/*

include $(DEPFILES)
