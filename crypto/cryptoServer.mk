##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=cryptoServer
ConfigurationName      :=Debug
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto"
ProjectPath            := "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Markeli
Date                   :=11.01.2014
CodeLitePath           :="/home/markeli/.codelite"
LinkerName             :=gcc
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=gcc -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=gcc
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=mkdir -p
CmpOptions             := -g $(Preprocessors)
LinkOptions            :=  
IncludePath            :=  "$(IncludeSwitch)." "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=
LibPath                := "$(LibraryPathSwitch)." 


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects=$(IntermediateDirectory)/myfingered$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)

makeDirStep:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/myfingered$(ObjectSuffix): myfingered.c $(IntermediateDirectory)/myfingered$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfingered.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/myfingered$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/myfingered$(DependSuffix): myfingered.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/myfingered$(ObjectSuffix) -MF$(IntermediateDirectory)/myfingered$(DependSuffix) -MM "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfingered.c"

$(IntermediateDirectory)/myfingered$(PreprocessSuffix): myfingered.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/myfingered$(PreprocessSuffix) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfingered.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/myfingered$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/myfingered$(DependSuffix)
	$(RM) $(IntermediateDirectory)/myfingered$(PreprocessSuffix)
	$(RM) $(OutputFile)


