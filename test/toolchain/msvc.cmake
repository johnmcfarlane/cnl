set(MISC_FLAGS "/W4 /WX /errorReport:prompt /nologo /wd4307")

set(EXCEPTION_ENABLED_FLAGS "/GR /EHsc")
set(EXCEPTION_DISABLED_FLAGS "/GR- -DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI")

set(INT128_ENABLED_FLAGS "")
set(INT128_DISABLED_FLAGS "")

set(SANITIZE_ENABLED_CXX_FLAGS "/fsanitize=address /Zi")
set(SANITIZE_ENABLED_LINKER_FLAGS "-LIBPATH:C:\\Program\ Files\ (x86)\\Microsoft\ Visual\ Studio\\2019\\Enterprise\\VC\\Tools\\Llvm\\x64\\lib\\clang\\10.0.0\\lib\\windows -WHOLEARCHIVE:clang_rt.asan_cxx-i386.lib -WHOLEARCHIVE:clang_rt.asan_dynamic-i386.lib -WHOLEARCHIVE:clang_rt.asan_dynamic_runtime_thunk-i386.lib")

set(SAMPLE_CXX_FLAGS "/wd4018")
