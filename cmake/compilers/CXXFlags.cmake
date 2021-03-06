if (NOT DEFINED DEFAULT_CXX_FLAGS_SET OR RESET_FLAGS)

# custom flags are defined by setup --custom-cxx-flags
if(DEFINED CUSTOM_CXX_FLAGS)
    # set custom compiler flags (for every build type)
    set(CMAKE_CXX_FLAGS "${CUSTOM_CXX_FLAGS}")
    # special flags for build types will be empty
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_PROFILE "")
else()
    # custom flags are not defined
    if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
       set(CMAKE_CXX_FLAGS "-DRESTRICT=${restrict} -Xlinker ${_exportdynamic}")
       if(HAS_CXX11_SUPPORT)
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CXX11_COMPILER_FLAGS}")
       else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++98")
       endif()
       # Special debug flags are set if ASan, TSan or UBSan are requested
       if(ENABLE_ASAN)
          set(CMAKE_CXX_FLAGS_DEBUG   "-g -O1 -fsanitize=address -fno-omit-frame-pointer")
       elseif(ENABLE_TSAN)
          set(CMAKE_CXX_FLAGS_DEBUG   "-g -O1 -fsanitize=thread -fno-omit-frame-pointer -pie")
       elseif(ENABLE_UBSAN)
          set(CMAKE_CXX_FLAGS_DEBUG   "-g -O1 -fsanitize=undefined -fno-omit-frame-pointer")
       else()
          set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3 -DDEBUG -Wall -Wextra -Winit-self -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Wwrite-strings")
       endif()
       set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wno-unused")
       set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -g -pg")
        
       if(ENABLE_CODE_COVERAGE)
          set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
	  set (CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fprofile-arcs -ftest-coverage")
       endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES Intel)
       set(CMAKE_CXX_FLAGS "-DRESTRICT=${restrict} -Xlinker ${_exportdynamic}")
       if(HAS_CXX11_SUPPORT)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CXX11_COMPILER_FLAGS}")
       else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++98")
       endif()
       set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -debug -DDEBUG -Wall -Wuninitialized -Wno-unknown-pragmas")
       # Check if xHost flag is available and add it CMAKE_CXX_FLAGS_RELEASE
       set(xHost "")
       check_cxx_compiler_flag("-xHost" has_xHost)
       if(has_xHost)
	  set(xHost "-xHost")
       endif()
       set(CMAKE_CXX_FLAGS_RELEASE "-O3 -no-prec-div -DNDEBUG ${xHost}")
       set(CMAKE_CXX_LINK_FLAGS    "${CMAKE_CXX_LINK_FLAGS} -shared-intel")
        if(DEFINED MKL_FLAG)
	   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MKL_FLAG}")
        endif()
       if(ENABLE_VECTORIZATION)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_ARCHITECTURE_FLAGS} ${DEFINITIONS}")
       endif()	      
         
       set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE} -g -pg")
       set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -shared-intel")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
       set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xlinker ${_exportdynamic}")
       set(CMAKE_CXX_FLAGS "-DRESTRICT=${restrict}")
       if(HAS_CXX11_SUPPORT)
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CXX11_COMPILER_FLAGS}")
       else()
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++98")
       endif()
       # Special debug flags are set if ASan, MSan, TSan or UBSan are requested
       if(ENABLE_ASAN)
          set(CMAKE_CXX_FLAGS_DEBUG    "-g -O1 -fsanitize=address -fno-omit-frame-pointer")
       elseif(ENABLE_MSAN)
          set(CMAKE_CXX_FLAGS_DEBUG    "-g -O1 -fsanitize=memory -fno-omit-frame-pointer")
       elseif(ENABLE_TSAN)
          set(CMAKE_CXX_FLAGS_DEBUG    "-g -O1 -fsanitize=thread -fno-omit-frame-pointer")
       elseif(ENABLE_UBSAN)
          set(CMAKE_CXX_FLAGS_DEBUG    "-g -O1 -fsanitize=undefined -fno-omit-frame-pointer")
       else()
           set(CMAKE_CXX_FLAGS_DEBUG    "-O0 -g -DDEBUG -Wall -Wextra -Winit-self -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Wwrite-strings -Weffc++ -Wdocumentation -Wno-unknown-pragmas")
       endif()
       set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -DNDEBUG -Wno-unused")

       # clang does not use gprof
       set(CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS_RELEASE}")
        
       if(ENABLE_CODE_COVERAGE)
          set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
	  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -fprofile-arcs -ftest-coverage")
       endif()
    else()
        message(FATAL_ERROR "Vendor of your C++ compiler is not supported")
    endif()

    if(DEFINED EXTRA_CXX_FLAGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
    endif()
endif()

save_compiler_flags(CXX)
endif()
