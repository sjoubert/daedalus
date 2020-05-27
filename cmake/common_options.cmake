set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)

include(CheckCXXCompilerFlag)
function(daedalus_add_compiler_flag p_flag)
  check_cxx_compiler_flag("${p_flag}" HAS_COMPILER_FLAG_${p_flag})
  if(HAS_COMPILER_FLAG_${p_flag})
    set(CMAKE_CXX_FLAGS "${p_flag} ${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
  endif()
endfunction()

daedalus_add_compiler_flag(-Wall)
daedalus_add_compiler_flag(-Wextra)
daedalus_add_compiler_flag(-Wshadow)
daedalus_add_compiler_flag(-Wunused-parameter)
daedalus_add_compiler_flag(-Wsuggest-override)
daedalus_add_compiler_flag(-Winconsistent-missing-override)
daedalus_add_compiler_flag(-Wzero-as-null-pointer-constant)
