#include <benchmark/benchmark.h>
#include <core/arithmetic/big_real.h>
#include <core/allocator/allocator.h>

class BigRealBench : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State &state) {
        const auto nbLimbsInt = state.range(0);
        const auto nbLimbsFrac = state.range(1);

        zparams = alloc.newObject<ZModuleParams<BigTorus>>(nbLimbsFrac);
        params = alloc.newObject<RealParams<BigTorus>>(zparams, nbLimbsInt);
        big_real_arr = alloc.newArray<BigReal>(big_real_arr_size, params, &alloc);
    }

    void TearDown(const ::benchmark::State &state) {
        alloc.deleteArray(big_real_arr_size, big_real_arr, params, &alloc);
        alloc.deleteObject(params);
        alloc.deleteObject(zparams);
    }

    Allocator alloc;
    ZModuleParams<BigTorus> *zparams = nullptr;
    RealParams<BigTorus> *params = nullptr;

    const int big_real_arr_size = 3;
    BigReal *big_real_arr = nullptr;
};

BENCHMARK_DEFINE_F(BigRealBench, add)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::add(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params);
    }
}

BENCHMARK_REGISTER_F(BigRealBench, add)->Ranges({{1, 64},
                                                 {1, 64}});
BENCHMARK_REGISTER_F(BigRealBench, add)->Args({1, 9})->Threads(1);


BENCHMARK_DEFINE_F(BigRealBench, mul_naive_round)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul_naive_round(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                                      alloc.createStackChildAllocator());
    }
}

BENCHMARK_REGISTER_F(BigRealBench, mul_naive_round)->Ranges({{1, 64},
                                                             {1, 64}});


BENCHMARK_DEFINE_F(BigRealBench, mul_naive_trunc)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul_naive_trunc(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                                      alloc.createStackChildAllocator());
    }
}

BENCHMARK_REGISTER_F(BigRealBench, mul_naive_trunc)->Ranges({{1, 64},
                                                             {1, 64}});


BENCHMARK_DEFINE_F(BigRealBench, mul)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                          alloc.createStackChildAllocator());
    }
}

BENCHMARK_REGISTER_F(BigRealBench, mul)->Ranges({{1, 64},
                                                 {1, 64}})->Threads(1);
BENCHMARK_REGISTER_F(BigRealBench, mul)->Args({1, 9})->Threads(1);

static void RealBench_add(benchmark::State &state) {
    uint64_t a, b;
    a = rand() * rand();
    b = rand() * rand();
    volatile uint64_t c;
    for (auto _ : state) {
        benchmark::DoNotOptimize(c = a + b);
    }
}

BENCHMARK(RealBench_add);

static void RealBench_mul(benchmark::State &state) {
    uint64_t a, b;
    a = rand() * rand();
    b = rand() * rand();
    volatile uint64_t c;
    for (auto _ : state) {
        benchmark::DoNotOptimize(c = a * b);
    }
}

BENCHMARK(RealBench_mul);


BENCHMARK_MAIN();

