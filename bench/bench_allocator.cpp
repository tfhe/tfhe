#include <benchmark/benchmark.h>
#include <core/allocator/allocator.h>

template<typename AllocatorTest>
static void Allocator_Bench_construct(benchmark::State& state) {
    AllocatorTest *alloc;
    for (auto _ : state) {
        benchmark::DoNotOptimize(alloc = new AllocatorTest());
    }
}
BENCHMARK_TEMPLATE(Allocator_Bench_construct, AllocatorImpl<TFHE_ALLOCATOR>);
BENCHMARK_TEMPLATE(Allocator_Bench_construct, AllocatorImpl<VALGRIND_ALLOCATOR>);

template<typename AllocatorTest>
static void Allocator_Bench_alloc_dealloc(benchmark::State& state) {
    const int nbBytes = state.range(0);
    uint8_t *arr = nullptr;
    AllocatorTest *alloc = new AllocatorTest();

    for (auto _ : state) {
        benchmark::DoNotOptimize(arr = (uint8_t*)alloc->allocate(0, nbBytes));
        alloc->deallocate(arr);
        benchmark::ClobberMemory();
    }

    delete alloc;
}
BENCHMARK_TEMPLATE(Allocator_Bench_alloc_dealloc, AllocatorImpl<TFHE_ALLOCATOR>)->Range(1<<6, 1<<16);
BENCHMARK_TEMPLATE(Allocator_Bench_alloc_dealloc, AllocatorImpl<VALGRIND_ALLOCATOR>)->Range(1<<6, 1<<16);

template<typename AllocatorTest>
static void Allocator_Bench_createStackChildAllocator(benchmark::State& state) {
    AllocatorTest alloc;
    for (auto _ : state) {
        alloc.createStackChildAllocator();
    }
}
BENCHMARK_TEMPLATE(Allocator_Bench_createStackChildAllocator, AllocatorImpl<TFHE_ALLOCATOR>);
BENCHMARK_TEMPLATE(Allocator_Bench_createStackChildAllocator, AllocatorImpl<VALGRIND_ALLOCATOR>);
