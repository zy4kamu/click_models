//#define COMPILER_MVS2010
//#define COMPILER_GPP47

#ifdef MSVC
    #define NOEXCEPT
#else
    #define NOEXCEPT noexcept
#endif
