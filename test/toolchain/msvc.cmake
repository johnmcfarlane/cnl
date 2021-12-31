set(CNL_CXX_FLAGS "/W4 /WX /constexpr:steps10000000 /errorReport:prompt /nologo /wd4307")

set(EXCEPTION_ENABLED_FLAGS "/GR /EHsc")
set(EXCEPTION_DISABLED_FLAGS "/GR- -DBOOST_NO_EXCEPTIONS -DBOOST_NO_RTTI")

set(INT128_ENABLED_FLAGS "")
set(INT128_DISABLED_FLAGS "")

set(SANITIZE_ENABLED_CXX_FLAGS "/fsanitize=address /fsanitize-address-use-after-return /RTCcsu -D_ALLOW_RTCc_IN_STL")

set(SAMPLE_CXX_FLAGS "/wd4018")
