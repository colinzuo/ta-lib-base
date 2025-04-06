
## official doc

[https://ta-lib.org/api/](https://ta-lib.org/api/)

## vs2022

IDE选择vs2022 community，没有特殊原因，就是当时这个是比较新的

另外IDE需要支持C17, pybind11里有用到

## ta_common project

新增project，选template `Static Library`，创建ta_common project，同时
创建ta_lib solution

vs2022不能指定solution不创建目录，那么如果想放到特定目录下就只能先创建，然后把里面
文件拷贝过去

缺省的项目配置有使能precompiled headers，这里我们用不到，所以删掉

### 添加headers and sources

添加源文件后C++配置才会出现

### 修改 Configuration

- Ouput Directory: `.\..\..\lib\`
- Target Name: `$(ProjectName)_$(Configuration)`
- Additional Include Directories: `..\..\include`
- Treat warnings as errors: `Yes`
- Precompiled Header: `Not Using Precompiled Headers`

## ta_func project

### 修改 Configuration

- Ouput Directory: `.\..\..\lib\`
- Target Name: `$(ProjectName)_$(Configuration)`
- Additional Include Directories: `.\..\..\src\ta_common;.\..\..\include`
- Treat warnings as errors: `Yes`
- Precompiled Header: `Not Using Precompiled Headers`

## ta_libc project

Makefile project，就是把前面两个lib给链接一起

### 修改 Configuration

不要在创建project时候配置比如build command，原因是如果在command里引用`OutDir`的话
则会出错，因为这里不能修改Output Directory，在创建project后修改时候在结果`.vcxproj`
文件中`OutDir`的修改排序在build command的后面，而这个文件看起来不支持这种情况。

- Output Directory: `..\..\lib\`
- Build Command Line: `lib /OUT:$(OutDir)ta_libc_$(Configuration).lib $(OutDir)ta_common_$(Configuration).lib $(OutDir)ta_func_$(Configuration).lib`
- Rebuild All Command Line: `lib /OUT:$(OutDir)ta_libc_$(Configuration).lib $(OutDir)ta_common_$(Configuration).lib $(OutDir)ta_func_$(Configuration).lib`
- Clean Command Line: `del $(OutDir)ta_libc_$(Configuration).lib`
- Output: `$(OutDir)ta_libc_$(Configuration).lib`

## ta_regtest project

exe project

### 修改 Configuration

- Output Directory: `.\..\..\bin\`
- Target Name: `$(ProjectName)_$(Configuration)`
- Treat warnings as errors: `Yes`
- Additional Include Directories: `.\..\..\src\ta_common;.\..\..\src\tools\ta_regtest;.\..\..\src\ta_func;.\..\..\include;`
- Additional Library Directories: `.\..\..\lib`
- Additional Dependencies: `ta_libc_$(Configuration).lib;$(CoreLibraryDependencies);%(AdditionalDependencies)`

## run ta_regtest

原来是在cpp侧做测试，个人倾向在python侧做测试，在cpp侧则只有在测试
失败时候做调试用。

运行ta_regtest验证工程能正常运行
