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
Date                   :=10.01.2014
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
Objects=$(IntermediateDirectory)/client$(ObjectSuffix) 

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
$(IntermediateDirectory)/client$(ObjectSuffix): client.c $(IntermediateDirectory)/client$(DependSuffix)
	$(C_CompilerName) $(SourceSwitch) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/client.c" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/client$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/client$(DependSuffix): client.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/client$(ObjectSuffix) -MF$(IntermediateDirectory)/client$(DependSuffix) -MM "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/client.c"

$(IntermediateDirectory)/client$(PreprocessSuffix): client.c
	@$(C_CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/client$(PreprocessSuffix) "/home/markeli/Documents/ITMO/SPL/Labs/Lab5/crypto/crypto/client.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/client$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/client$(DependSuffix)
	$(RM) $(IntermediateDirectory)/client$(PreprocessSuffix)
	$(RM) $(OutputFile)


