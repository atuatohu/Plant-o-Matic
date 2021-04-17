cmake_minimum_required(VERSION 3.1.0)

project (Plant)
set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(.)
include_directories(src)
include_directories(test)


find_library(wiringPi_LIB wiringPi)


add_executable(Plant src/demo.cpp src/ultrasonic.cpp src/MCP3008.cpp src/DHT11.cpp)
add_executable(unit_test_ultra test/ultra_test.cpp src/ultrasonic.cpp)
add_executable(unit_test_soil test/soil_test.cpp src/MCP3008.cpp)
add_executable(unit_test_pump test/soil_pump_test.cpp src/MCP3008.cpp)
add_executable(unit_test_low test/ultra_pump_test.cpp src/ultrasonic.cpp)
add_executable(unit_test_dht test/dht_test.cpp src/DHT11.cpp)

TARGET_LINK_LIBRARIES(Plant ${wiringPi_LIB})

TARGET_LINK_LIBRARIES(unit_test_ultra ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(unit_test_soil ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(unit_test_pump ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(unit_test_low ${wiringPi_LIB})
TARGET_LINK_LIBRARIES(unit_test_dht ${wiringPi_LIB})
enable_testing()

add_test(NAME Test_for_ultra COMMAND unit_test_ultra)
add_test(NAME Test_for_soil COMMAND unit_test_soil)
add_test(NAME Test_for_pump COMMAND unit_test_pump)
add_test(NAME Test_for_low COMMAND unit_test_low)
add_test(NAME Test_for_dht COMMAND unit_test_dht)