# Copyright (C) 2019, International Business Machines
# Corporation.  All Rights Reserved.

# This program is distributed under the terms of the
# Eclipse Public License - v 2.0

ROOT   = $(PWD)

CXX    := g++

ifeq ($(shell uname),SunOS)
	CXX := CC
endif

IBM_SRCS :=
IBM_SRCS += Vec.cpp
IBM_SRCS += Alg1.cpp
IBM_SRCS += Alg7.cpp
IBM_SRCS += Alg8.cpp
IBM_SRCS += Alg8p.cpp
IBM_SRCS += Alg5.cpp
IBM_SRCS += Alg5p.cpp
IBM_SRCS += Alg6.cpp
IBM_SRCS += Alg4.cpp
IBM_SRCS += Alg9.cpp
IBM_SRCS += BuildMatA.cpp
IBM_SRCS += readData.cpp





CXXFLAGS  = -Ofast
CXXFLAGS += -I$(ROOT)


###############################################################################

# CXXOPT = -O -pg
# CXXOPT = -O -g
# CXXOPT = -g
CXXOPT = -Ofast

###############################################################################

ifeq ($(findstring g++, $(CXX)),g++)
	DEPEXT = d
	CXXFLAGS += -Wall
	CXXFLAGS += -MMD -MT $(OBJDIR)/$*.dep
	SHLIBPREFIX = -Wl,-rpath,
endif

ifeq ($(findstring CC, $(CXX)),CC)
	DEPEXT = i
	SHLIBPREFIX = -R
endif

###############################################################################

OBJDIR = obj

LIBS  =
#LIBS += -lemon

###############################################################################

VPATH :=

###############################################################################

EXTRA_SRCS := 

SRCS := $(IBM_SRCS) $(EXTRA_SRCS)

###############################################################################

# EXTRAOPT = -O -pg
# EXTRAOPT = -O -g
EXTRAOPT = -Ofast

###############################################################################

IBM_OBJS = $(addprefix $(OBJDIR)/,$(IBM_SRCS:.cpp=.o))
IBM_DEPS = PHONY $(addprefix $(OBJDIR)/,$(IBM_SRCS:.cpp=.$(DEPEXT)))
IBM_DEPINDS = $(addprefix $(OBJDIR)/,$(IBM_SRCS:.cpp=.dep))

EXTRA_OBJS = $(addprefix $(OBJDIR)/,$(EXTRA_SRCS:.cpp=.o))
EXTRA_DEPS = PHONY $(addprefix $(OBJDIR)/,$(EXTRA_SRCS:.cpp=.$(DEPEXT)))
EXTRA_DEPINDS = $(addprefix $(OBJDIR)/,$(EXTRA_SRCS:.cpp=.dep))

CURRENTDEPS = $(notdir $(wildcard $(OBJDIR)/*.$(DEPEXT)))
CURRENTDEPINDS = $(notdir $(wildcard $(OBJDIR)/*.dep))
CURRENTOBJS = $(notdir $(wildcard $(OBJDIR)/*.o))

MISSING  = $(filter-out $(CURRENTDEPS:.$(DEPEXT)=),$(SRCS:.cpp=))
MISSING += $(filter-out $(CURRENTDEPINDS:.dep=),$(SRCS:.cpp=))
MISSING += $(filter-out $(CURRENTOBJS:.o=),$(SRCS:.cpp=))

NO_DEPS = $(addprefix $(OBJDIR)/,$(sort $(MISSING)))

###############################################################################
# PHONY and default targets

default : driver_heur
all: default
.SECONDARY: $(DEPINDS)

###############################################################################
# final targets
.PHONY : cleantest driver_heur
.SECONDARY : $(OBJDIR)/main.dep
driver_heur : cleanobj cleantest $(IBM_OBJS) $(EXTRA_OBJS) $(OBJDIR)/driver_heur.o
	rm -f $@
	$(CXX) $(CXXOPT) $(IBM_OBJS) $(EXTRA_OBJS) $(OBJDIR)/driver_heur.o -o $@ $(LIBS)





.PHONY : cleanall
cleanall :
	rm -f $(OBJDIR)/* 

.PHONY : clean
clean :
	rm -f $(addprefix $(OBJDIR)/,$(IBM_SRCS:.cpp=.*))

.PHONY : cleanobj
cleanobj:
	rm -f $(addsuffix .*,$(NO_DEPS))

###############################################################################
# compilation targets

$(OBJDIR)/%.o : $(OBJDIR)/%.dep ;

$(IBM_DEPINDS) $(OBJDIR)/driver_heur.dep : $(OBJDIR)/%.dep : %.cpp
	@mkdir -p $(OBJDIR)
ifeq ($(findstring CC, $(CXX)),CC)
	$(CXX) -xM1 -c $(CXXFLAGS) $(CXXOPT) $< | \
		sed -e 's/(.*)\.o :/$1.dep :/g' > $(OBJDIR)/$*.$(DEPEXT)
endif
	$(CXX) -c $(CXXFLAGS) $(CXXOPT) $< -o $(OBJDIR)/$*.o
	touch $@

$(EXTRA_DEPINDS) : $(OBJDIR)/%.dep : %.cpp
	@mkdir -p $(OBJDIR)
ifeq ($(findstring CC, $(CXX)),CC)
	$(CXX) -xM1 -c $(CXXFLAGS) $(EXTRAOPT) $< | \
		sed -e 's/(.*)\.o :/$1.dep :/g' > $(OBJDIR)/$*.$(DEPEXT)
endif
	$(CXX) -c $(CXXFLAGS) $(EXTRAOPT) $< -o $(OBJDIR)/$*.o
	touch $@

-include $(IBM_DEPS)
-include $(EXTRA_DEPS)
