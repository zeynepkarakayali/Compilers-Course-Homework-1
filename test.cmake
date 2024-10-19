
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

#
# tests
#

add_executable(test_arithmetic kiraz/test/test_arithmetic.cc)
target_link_libraries(test_arithmetic kiraz GTest::gtest_main ${FLEX_LIBRARIES})
gtest_discover_tests(test_arithmetic)
