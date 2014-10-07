#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include nodewalker-Makefile.mk

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/198669854/node.o \
	${OBJECTDIR}/_ext/198669854/error.o \
	${OBJECTDIR}/_ext/198669854/simulation.o \
	${OBJECTDIR}/_ext/198669854/partialgraph.o \
	${OBJECTDIR}/_ext/198669854/walker.o \
	${OBJECTDIR}/_ext/198669854/random.o \
	${OBJECTDIR}/_ext/198669854/graph.o \
	${OBJECTDIR}/_ext/198669854/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodewalker

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodewalker: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodewalker ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/198669854/node.o: /Users/andy/Documents/Dropbox/codes/nodewalker/node.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/node.o /Users/andy/Documents/Dropbox/codes/nodewalker/node.cpp

${OBJECTDIR}/_ext/198669854/error.o: /Users/andy/Documents/Dropbox/codes/nodewalker/error.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/error.o /Users/andy/Documents/Dropbox/codes/nodewalker/error.cpp

${OBJECTDIR}/_ext/198669854/simulation.o: /Users/andy/Documents/Dropbox/codes/nodewalker/simulation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/simulation.o /Users/andy/Documents/Dropbox/codes/nodewalker/simulation.cpp

${OBJECTDIR}/_ext/198669854/partialgraph.o: /Users/andy/Documents/Dropbox/codes/nodewalker/partialgraph.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/partialgraph.o /Users/andy/Documents/Dropbox/codes/nodewalker/partialgraph.cpp

${OBJECTDIR}/_ext/198669854/walker.o: /Users/andy/Documents/Dropbox/codes/nodewalker/walker.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/walker.o /Users/andy/Documents/Dropbox/codes/nodewalker/walker.cpp

${OBJECTDIR}/_ext/198669854/random.o: /Users/andy/Documents/Dropbox/codes/nodewalker/random.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/random.o /Users/andy/Documents/Dropbox/codes/nodewalker/random.cpp

${OBJECTDIR}/_ext/198669854/graph.o: /Users/andy/Documents/Dropbox/codes/nodewalker/graph.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/graph.o /Users/andy/Documents/Dropbox/codes/nodewalker/graph.cpp

${OBJECTDIR}/_ext/198669854/main.o: /Users/andy/Documents/Dropbox/codes/nodewalker/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/198669854
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/198669854/main.o /Users/andy/Documents/Dropbox/codes/nodewalker/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nodewalker

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
