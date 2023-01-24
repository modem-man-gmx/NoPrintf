##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=NoPrintf
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :=D:/proj/proj.github/NoPrintf
ProjectPath            :=D:/proj/proj.github/NoPrintf
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Maik
Date                   :=24/01/2023
CodeLitePath           :=C:/c/CodeLite
LinkerName             :=C:/c/msys64/mingw64/bin/g++.exe
SharedObjectLinkerName :=C:/c/msys64/mingw64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=$(IntermediateDirectory)
OutputFile             :=$(IntermediateDirectory)/$(ProjectName).exe
Preprocessors          :=$(PreprocessorSwitch)DEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="NoPrintf.txt"
PCHCompileFlags        :=
MakeDirCommand         :=C:/c/CodeLite/mkdir.exe -p
RcCmpOptions           := 
RcCompilerName         :=C:/c/msys64/mingw64/bin/windres.exe
LinkOptions            :=  -pg
IncludePath            := $(IncludeSwitch)/c/c/msys64/mingw64/include  $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                :=$(LibraryPathSwitch)/c/c/msys64/mingw64/lib  $(LibraryPathSwitch). $(LibraryPathSwitch)c:\c\msys64\mingw64\lib\ $(LibraryPathSwitch)c:\c\msys64\mingw64\bin\ 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/c/msys64/mingw64/bin/ar.exe rcu
CXX      := C:/c/msys64/mingw64/bin/g++.exe
CC       := C:/c/msys64/mingw64/bin/gcc.exe
CXXFLAGS :=  -Wfatal-errors -pg -g -pedantic -W -std=c++14  -DSVN_REVISION=\"\"  $(Preprocessors)
CFLAGS   :=  -Wfatal-errors -pg -g -pedantic -W -std=c99  -DSVN_REVISION=\"\"  $(Preprocessors)
ASFLAGS  := 
AS       := C:/c/msys64/mingw64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\c\msys64\mingw64\share\codelite
Objects0=$(IntermediateDirectory)/src_no_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_no_printf.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(ConfigurationName)"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(ConfigurationName)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_no_main.cpp$(ObjectSuffix): src/no_main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_no_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_no_main.cpp$(DependSuffix) -MM src/no_main.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/proj/proj.github/NoPrintf/src/no_main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_no_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_no_main.cpp$(PreprocessSuffix): src/no_main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_no_main.cpp$(PreprocessSuffix) src/no_main.cpp

$(IntermediateDirectory)/src_no_printf.cpp$(ObjectSuffix): src/no_printf.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_no_printf.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_no_printf.cpp$(DependSuffix) -MM src/no_printf.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "D:/proj/proj.github/NoPrintf/src/no_printf.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_no_printf.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_no_printf.cpp$(PreprocessSuffix): src/no_printf.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_no_printf.cpp$(PreprocessSuffix) src/no_printf.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


