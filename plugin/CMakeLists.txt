cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(HENkaku)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,-q -Wall -fshort-wchar -O3 -std=gnu99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti -fno-exceptions")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/henkaku-stubs
)

if (NOT ${RELEASE})
  add_definitions(-DENABLE_LOGGING)
endif()

# Builds
function(ADD_RESOURCES out_var)
  set(result)
  foreach(in_f ${ARGN})
    set(out_f "${CMAKE_CURRENT_BINARY_DIR}/${in_f}.o")
    get_filename_component(out_dir ${out_f} DIRECTORY)
    add_custom_command(OUTPUT ${out_f}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${out_dir}
      COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_f} ${in_f}
      DEPENDS ${in_f}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Building resource ${out_f}"
      VERBATIM
      )
    list(APPEND result ${out_f})
  endforeach()
  set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

file(GLOB res_files RELATIVE ${CMAKE_SOURCE_DIR} *.xml)
add_resources(xml_res ${res_files})

add_executable(user
  ${xml_res}
  user.c
)

add_executable(kernel
  kernel.c
)

target_link_libraries(user
  taihen_stub
  HENkaku_stub
  SceLibKernel_stub
  SceIofilemgr_stub
  SceRegistryMgr_stub
  ScePower_stub
  SceVshBridge_stub
)

target_link_libraries(kernel
  gcc
  taihenForKernel_stub
  SceSysmemForDriver_stub
  SceSysclibForDriver_stub
  SceIofilemgrForDriver_stub
  SceDebugForDriver_stub
  SceThreadmgrForDriver_stub
)

add_dependencies(user henkaku-stubs)

set_target_properties(kernel
  PROPERTIES LINK_FLAGS "-nostdlib"
  COMPILE_FLAGS "-D__VITA_KERNEL__"
)

set_target_properties(user
  PROPERTIES LINK_FLAGS "-nostdlib"
)

vita_create_self(henkaku.skprx kernel
  UNSAFE
  CONFIG ${CMAKE_SOURCE_DIR}/kernel.yml
)
vita_create_stubs(henkaku-stubs kernel ${CMAKE_SOURCE_DIR}/kernel.yml
  KERNEL
)
vita_create_self(henkaku.suprx user
  UNSAFE
  CONFIG ${CMAKE_SOURCE_DIR}/user.yml
)

install(DIRECTORY ${CMAKE_BINARY_DIR}/henkaku-stubs/
  DESTINATION lib
  FILES_MATCHING PATTERN "*.a"
)
