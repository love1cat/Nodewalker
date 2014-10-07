## -*- Makefile -*-
##
## User: andy
## Time: Dec 12, 2012 10:05:49 PM
## Makefile created by Oracle Solaris Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
CCC = /apps/openmpi/1.6.1/bin/mpic++
CXX = /apps/openmpi/1.6.1/bin/mpic++
BASICOPTS = -g
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 


# Define the target directories.
TARGETDIR_nwsim=.


all: nwsim

## Target: nwsim
CPPFLAGS_nwsim = \
	-I/apps/openmpi/1.6.1/include/ \
	-I/apps/boost/1.49/include/ \
	-L/apps/openmpi/1.6.1/lib/ \
	-fopenmp
OBJS_nwsim =  \
	$(TARGETDIR_nwsim)/error.o \
	$(TARGETDIR_nwsim)/graph.o \
	$(TARGETDIR_nwsim)/main.o \
	$(TARGETDIR_nwsim)/node.o \
	$(TARGETDIR_nwsim)/partialgraph.o \
	$(TARGETDIR_nwsim)/random.o \
	$(TARGETDIR_nwsim)/simulation.o \
	$(TARGETDIR_nwsim)/walker.o
USERLIBS_nwsim = $(SYSLIBS_nwsim) 
DEPLIBS_nwsim =   
LDLIBS_nwsim = $(USERLIBS_nwsim)


# Link or archive
$(TARGETDIR_nwsim)/nwsim: $(TARGETDIR_nwsim) $(OBJS_nwsim) $(DEPLIBS_nwsim)
	$(LINK.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ $(OBJS_nwsim) $(LDLIBS_nwsim)


# Compile source files into .o files
$(TARGETDIR_nwsim)/error.o: $(TARGETDIR_nwsim) error.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ error.cpp

$(TARGETDIR_nwsim)/graph.o: $(TARGETDIR_nwsim) graph.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ graph.cpp

$(TARGETDIR_nwsim)/main.o: $(TARGETDIR_nwsim) main.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ main.cpp

$(TARGETDIR_nwsim)/node.o: $(TARGETDIR_nwsim) node.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ node.cpp

$(TARGETDIR_nwsim)/partialgraph.o: $(TARGETDIR_nwsim) partialgraph.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ partialgraph.cpp

$(TARGETDIR_nwsim)/random.o: $(TARGETDIR_nwsim) random.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ random.cpp

$(TARGETDIR_nwsim)/simulation.o: $(TARGETDIR_nwsim) simulation.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ simulation.cpp

$(TARGETDIR_nwsim)/walker.o: $(TARGETDIR_nwsim) walker.cpp
	$(COMPILE.cc) $(CCFLAGS_nwsim) $(CPPFLAGS_nwsim) -o $@ walker.cpp



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_nwsim)/nwsim \
		$(TARGETDIR_nwsim)/error.o \
		$(TARGETDIR_nwsim)/graph.o \
		$(TARGETDIR_nwsim)/main.o \
		$(TARGETDIR_nwsim)/node.o \
		$(TARGETDIR_nwsim)/partialgraph.o \
		$(TARGETDIR_nwsim)/random.o \
		$(TARGETDIR_nwsim)/simulation.o \
		$(TARGETDIR_nwsim)/walker.o
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-x86_64-Linux

