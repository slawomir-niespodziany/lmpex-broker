cmake_minimum_required(VERSION 3.22)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(broker ${SRC_FILES})

target_link_libraries(broker lmf abstract_link 
                      "-Wl,--push-state,--whole-archive,$<TARGET_FILE:eth_link> -Wl,--pop-state"
                      "-Wl,--push-state,--whole-archive,$<TARGET_FILE:gsm_link> -Wl,--pop-state"
                      "-Wl,--push-state,--whole-archive,$<TARGET_FILE:sat_link> -Wl,--pop-state"
)
