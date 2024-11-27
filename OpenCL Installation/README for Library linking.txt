<!-- @format -->

Modified Project Properties

1. Copy "OpenCL" folder to "C:\"

2. Modify project properties. From "Project" >
   1. Configuration Properties -> C/C++ -> General -> Additional Include Directories
      Add: C:\OpenCL\sdk\include;
   2. Configuration Properties -> Linker -> Input -> Additional Dependencies
      From: $(CoreLibraryDependencies);%(AdditionalDependencies)
        To: C:\OpenCL\sdk\lib\x64\OpenCL.lib;C:\OpenCL\sdk\lib\x86\OpenCL.lib;$(CoreLibraryDependencies);%(AdditionalDependencies);


Note
Original Skeletal Code use 2.0.10 cl2.hpp
 - Support x86 platform

Attempt 