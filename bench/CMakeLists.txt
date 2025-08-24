find_package(benchmark REQUIRED)

# rxcpp benchmark 
add_executable(rxcpp_bench ${CMAKE_CURRENT_SOURCE_DIR}/rxcpp/bench.cc)
find_package(rxcpp REQUIRED)
target_link_libraries(
    rxcpp_bench PRIVATE
    benchmark::benchmark_main
    rxcpp::rxcpp
    pthread
)
target_include_directories(
    rxcpp_bench INTERFACE
    ${rxcpp_INCLUDE_DIR}
)

# reaction benchmark
file(GLOB BENCH_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)
foreach(bench_file ${BENCH_SRC})
    get_filename_component(bench_file_name ${bench_file} NAME_WE)
    add_executable(${bench_file_name} ${bench_file}) 
    target_link_libraries(${bench_file_name} PRIVATE benchmark::benchmark_main ${PROJECT_NAME} pthread)
endforeach()
