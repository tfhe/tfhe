#include <benchmark/benchmark.h>
#include <core/allocator/allocator.h>

template<typename AllocatorTest>
static void Allocator_Bench_con_decon(benchmark::State& state) {
    AllocatorTest *alloc;
    for (auto _ : state) {
        alloc = new AllocatorTest();
        delete alloc;
    }
}
BENCHMARK_TEMPLATE(Allocator_Bench_con_decon, AllocatorImpl<TFHE_ALLOCATOR>);
BENCHMARK_TEMPLATE(Allocator_Bench_con_decon, AllocatorImpl<VALGRIND_ALLOCATOR>);

template<typename AllocatorTest>
static void Allocator_Bench_alloc_dealloc(benchmark::State& state) {
    const int nbBytes = state.range(0);
    uint8_t *arr = nullptr;
    AllocatorTest *alloc = new AllocatorTest();

    for (auto _ : state) {
        arr = (uint8_t*)alloc->allocate(1, nbBytes);
        alloc->deallocate(arr);
    }

    delete alloc;
}
BENCHMARK_TEMPLATE(Allocator_Bench_alloc_dealloc, AllocatorImpl<TFHE_ALLOCATOR>)->Range(1<<6, 1<<20);
BENCHMARK_TEMPLATE(Allocator_Bench_alloc_dealloc, AllocatorImpl<VALGRIND_ALLOCATOR>)->Range(1<<6, 1<<20);

template<typename AllocatorTest>
static void Allocator_Bench_createStackChildAllocator(benchmark::State& state) {
    AllocatorTest *alloc = new AllocatorTest();
    for (auto _ : state) {
        AllocatorTest child_alloc = alloc->createStackChildAllocator();
    }
    delete alloc;
}
BENCHMARK_TEMPLATE(Allocator_Bench_createStackChildAllocator, AllocatorImpl<TFHE_ALLOCATOR>);
BENCHMARK_TEMPLATE(Allocator_Bench_createStackChildAllocator, AllocatorImpl<VALGRIND_ALLOCATOR>);
