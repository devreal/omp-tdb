
#define XSTR(x) #x
#define STR(x) XSTR(x)

#if defined(_CRAYC)
#define COMPILER_STR "CRAY-" STR(_RELEASE) "." STR(_RELEASE_MINOR)
#elif defined(__clang__)
#define COMPILER_STR "CLANG-" STR(__clang_major__) "." STR(__clang_minor__) "." STR(__clang_patchlevel__)
#elif defined(__PGI)
#define COMPILER_STR "PGI-" STR(__PGIC__) "." STR(__PGIC_MINOR__) "." STR(__PGIC_PATCHLEVEL__)
#elif defined(__INTEL_COMPILER)
#define COMPILER_STR "ICC-" STR(__INTEL_COMPILER)
#elif defined(__GNUC__)
#define COMPILER_STR "GNU-" STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
#else
#error "Unknown compiler detected!"
#endif
