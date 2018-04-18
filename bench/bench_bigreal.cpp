#include <benchmark/benchmark.h>
#include <core/arithmetic/big_real.h>
#include <core/allocator/allocator.h>

class BigReal_Bench : public benchmark::Fixture {
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

// void add(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigReal_Bench, add)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::add(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params);
    }
}
BENCHMARK_REGISTER_F(BigReal_Bench, add)->Ranges({{1, 64},
                                                 {1, 64}});

// void mul_naive_round(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
//                      Allocator alloc);
BENCHMARK_DEFINE_F(BigReal_Bench, mul_naive_round)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul_naive_round(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                                      &alloc);
    }
}
BENCHMARK_REGISTER_F(BigReal_Bench, mul_naive_round)->Ranges({{1, 64},
                                                             {1, 64}});


// void mul_naive_trunc(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params,
//                      Allocator alloc);
BENCHMARK_DEFINE_F(BigReal_Bench, mul_naive_trunc)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul_naive_trunc(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                                      &alloc);
    }
}
BENCHMARK_REGISTER_F(BigReal_Bench, mul_naive_trunc)->Ranges({{1, 64},
                                                             {1, 64}});


// void mul(BigReal *res, const BigReal *a, const BigReal *b, const RealParams<BigTorus> *params, Allocator alloc);
BENCHMARK_DEFINE_F(BigReal_Bench, mul)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::mul(big_real_arr + 2, big_real_arr + 1, big_real_arr + 0, params,
                          &alloc);
    }
}
BENCHMARK_REGISTER_F(BigReal_Bench, mul)->Ranges({{1, 64},
                                                 {1, 64}});

static void PrimitiveInt64_add(benchmark::State &state) {
    uint64_t a, b;
    a = rand() * rand();
    b = rand() * rand();
    volatile uint64_t c;
    for (auto _ : state) {
        benchmark::DoNotOptimize(c = a + b);
    }
}
BENCHMARK(PrimitiveInt64_add);

static void PrimitiveInt64_mul(benchmark::State &state) {
    uint64_t a, b;
    a = rand() * rand();
    b = rand() * rand();
    volatile uint64_t c;
    for (auto _ : state) {
        benchmark::DoNotOptimize(c = a * b);
    }
}
BENCHMARK(PrimitiveInt64_mul);

