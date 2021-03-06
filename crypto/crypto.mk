##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=crypto
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
Objects=$(IntermediateDirectory)/myfinger$(ObjectSuffix) 

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
$(IntermediateDirectory)/myfinger$(ObjectSuffix): myfinger.c $(IntermediateDirectory)/myfinger$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfinger.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/myfinger$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/myfinger$(DependSuffix): myfinger.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/myfinger$(ObjectSuffix) -MF$(IntermediateDirectory)/myfinger$(DependSuffix) -MM "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfinger.c"

$(IntermediateDirectory)/myfinger$(PreprocessSuffix): myfinger.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/myfinger$(PreprocessSuffix) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/myfinger.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/myfinger$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/myfinger$(DependSuffix)
	$(RM) $(IntermediateDirectory)/myfinger$(PreprocessSuffix)
	$(RM) $(OutputFile)


