cmake_minimum_required(VERSION 3.22)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(broker ${SRC_FILES})

target_link_libraries(broker lmf abstract_link)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(broker 
                        "-Wl,--push-state,--whole-archive,$<TARGET_FILE:eth_link> -Wl,--pop-state"
                        "-Wl,--push-state,--whole-archive,$<TARGET_FILE:gsm_link> -Wl,--pop-state"
                        "-Wl,--push-state,--whole-archive,$<TARGET_FILE:sat_link> -Wl,--pop-state"
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") 
    target_link_libraries(broker eth_link gsm_link sat_link)
    set_target_properties(broker PROPERTIES LINK_FLAGS_DEBUG "/WHOLEARCHIVE:eth_link /WHOLEARCHIVE:gsm_link /WHOLEARCHIVE:sat_link")

else()
    message(FATAL_ERROR "Unsupported compiler." )
endif()




message("CXX: ${CMAKE_CXX_COMPILER_ID}")