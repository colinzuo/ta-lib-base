
## official doc

<https://ta-lib.org/d_api/d_api.html>

## vs2022

复制vs2005到目录vs2022
使用vs2022打开

### UpgradeLog.htm

自动升级会有一些报警，比如`$(TargetPath)`和`OutputFile`路径不一致

<https://learn.microsoft.com/en-us/cpp/build/reference/common-macros-for-build-commands-and-properties?view=msvc-170>

Linker里的Output File的缺省值是`$(OutDir)$(TargetName)$(TargetExt)`

`$(TargetPath)`应该预期和这个一致，但实际不一样，所以警告
(vs2019版本不会警告)

MSB8012: $(TargetPath) ('C:\work3\repos\myself\ta-lib-base\ta-lib\c\bin\gen_code.exe') does not match the Librarian's OutputFile property value '.\..\..\..\..\bin\gen_code.exe' ('C:\work3\repos\myself\ta-lib-base\ta-lib\bin\gen_code.exe') in project configuration 'cmr|Win32'. This may cause your project to build incorrectly. To correct this, please make sure that $(TargetPath) property value matches the value specified in %(Lib.OutputFile).

MSB8012: $(TargetName) ('gen_code') does not match the Linker's OutputFile property value '.\..\..\..\..\bin\gen_code_cmr.exe' ('gen_code_cmr') in project configuration 'cmr|Win32'. This may cause your project to build incorrectly. To correct this, please make sure that $(TargetName) property value matches the value specified in %(Link.OutputFile).

MSB8012: $(TargetPath) ('C:\work3\repos\myself\ta-lib-base\ta-lib\c\bin\gen_code.exe') does not match the Linker's OutputFile property value '.\..\..\..\..\bin\gen_code_cmr.exe' ('C:\work3\repos\myself\ta-lib-base\ta-lib\bin\gen_code_cmr.exe') in project configuration 'cmr|Win32'. This may cause your project to build incorrectly. To correct this, please make sure that $(TargetPath) property value matches the value specified in %(Link.OutputFile).

### 和default project的配置区别

(对比xx.vcxproj文件)

Advanced
- Use Debug Libraries: default debug里是`Yes`，这里都被设成`No`了
- Character Set: default里是`Use Unicode Character Set`，这里被设成`Use Multi-Byte Character Set`
- Whole Program Optimization: default release里是`Use Link Time Code Generation`, 这里被设成`No Whole Program Optimization`

```
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='cmr|Win32'">
    <OutDir>.\..\..\..\..\lib\</OutDir>
    <IntDir>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)\</IntDir>
    <TargetName>$(ProjectName)_$(Configuration)</TargetName>
  </PropertyGroup>
```

#### cdd|Win32 vs Debug|Win32

```xml
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='cdd|Win32'">
    <ClCompile>
      <Optimization>Disabled</Optimization>
      <AdditionalIncludeDirectories>..\..\..\..\include;..\..\..\..\src\ta_common\imatix\sfl;..\..\..\..\src\ta_common;..\..\..\..\src\ta_common\mt;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
      <PreprocessorDefinitions>TA_DEBUG;_DEBUG;WIN32;_MBCS;_LIB;QT_THREAD_SUPPORT;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary>
      <PrecompiledHeaderOutputFile>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/$(ProjectName).pch</PrecompiledHeaderOutputFile>
      <AssemblerListingLocation>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/</AssemblerListingLocation>
      <ObjectFileName>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/</ObjectFileName>
      <ProgramDataBaseFileName>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/</ProgramDataBaseFileName>
      <XMLDocumentationFileName>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/</XMLDocumentationFileName>
      <BrowseInformation>true</BrowseInformation>
      <BrowseInformationFile>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/</BrowseInformationFile>
      <WarningLevel>Level3</WarningLevel>
      <TreatWarningAsError>true</TreatWarningAsError>
      <SuppressStartupBanner>true</SuppressStartupBanner>
      <DebugInformationFormat>ProgramDatabase</DebugInformationFormat>
    </ClCompile>
    <ResourceCompile>
      <PreprocessorDefinitions>NDEBUG;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <Culture>0x0409</Culture>
    </ResourceCompile>
    <Lib>
      <OutputFile>.\..\..\..\..\lib/$(ProjectName)_$(Configuration).lib</OutputFile>
      <SuppressStartupBanner>true</SuppressStartupBanner>
    </Lib>
    <Xdcmake>
      <OutputFile>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/$(ProjectName)_$(Configuration).xml</OutputFile>
    </Xdcmake>
    <Bscmake>
      <SuppressStartupBanner>true</SuppressStartupBanner>
      <OutputFile>.\..\..\..\..\temp\$(Configuration)\$(ProjectName)/$(ProjectName)_$(Configuration).bsc</OutputFile>
    </Bscmake>
  </ItemDefinitionGroup>
```

```xml
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <SDLCheck>true</SDLCheck>
      <PreprocessorDefinitions>WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ConformanceMode>true</ConformanceMode>
      <PrecompiledHeader>Use</PrecompiledHeader>
      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>
    </ClCompile>
    <Link>
      <SubSystem>
      </SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
    </Link>
  </ItemDefinitionGroup>
```  

## 新增x64

在toolbar的Win32处选择Configuration Manger
在solution platform处选择New添加x64

x64的project properties里会被自动配置

### Librarian

General
- Target Machine: `MachineX64 (/MACHINE:X64)`

## ta_common project properties

### C/C++ 

General
- Additional Include Directories: `..\..\..\..\include;..\..\..\..\src\ta_common\imatix\sfl;..\..\..\..\src\ta_common;..\..\..\..\src\ta_common\mt;%(AdditionalIncludeDirectories)`
Preprocessor
- Preprocessor Definitions: `TA_DEBUG;_DEBUG;WIN32;_MBCS;_LIB;QT_THREAD_SUPPORT;%(PreprocessorDefinitions)`
Code Generation
- Runtime Library: `Multi-threaded Debug DLL (/MDd)`

### Librarian

General
- Output File: `.\..\..\..\..\lib/$(ProjectName)_$(Configuration).lib`
