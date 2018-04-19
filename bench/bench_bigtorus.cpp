#include <benchmark/benchmark.h>
#include <core/arithmetic/big_torus.h>
#include <core/arithmetic/big_int.h>
#include <core/allocator/allocator.h>

class BigTorus_Bench1 : public benchmark::Fixture {
public:
    virtual void BuildParams(const ::benchmark::State &state) {
        params = alloc.newObject<ZModuleParams<BigTorus>>(state.range(0));
    }

    virtual void BuildAuxData() {
        big_torus_arr = alloc.newArray<BigTorus>(big_torus_arr_size, params, &alloc);
    }

    void SetUp(const ::benchmark::State &state) {
        BuildParams(state);
        BuildAuxData();
    }

    virtual void DeleteAuxData() {
        alloc.deleteArray(big_torus_arr_size, big_torus_arr, params, &alloc);
    }

    void TearDown(const ::benchmark::State &state) {
        DeleteAuxData();
        alloc.deleteObject(params);
    }

    Allocator alloc;
    ZModuleParams<BigTorus> *params = nullptr;

    const int big_torus_arr_size = 3;
    BigTorus *big_torus_arr = nullptr;
};

class BigTorus_Bench2 : public BigTorus_Bench1 {
public:
    BigTorus_Bench2() {
        gmp_randinit_default(rnd_state);
    }

    ~BigTorus_Bench2() {
        gmp_randclear(rnd_state);
    }

    virtual void BuildParams(const ::benchmark::State &state) {
        params = alloc.newObject<ZModuleParams<BigTorus>>(state.range(1), state.range(0)*64);
    }

    virtual void BuildAuxData() {
        BigTorus_Bench1::BuildAuxData();
        big_int_arr = alloc.newArray<BigInt>(big_int_arr_size);
        for (int i = 0; i < big_int_arr_size; ++i)
            mpz_urandomb(big_int_arr[i].data, rnd_state, params->p);
    }

    virtual void DeleteAuxData() {
        alloc.deleteArray(big_int_arr_size, big_int_arr);
        BigTorus_Bench1::DeleteAuxData();
    }

    const int big_int_arr_size = 1;
    BigInt *big_int_arr = nullptr;
    gmp_randstate_t rnd_state;
};

// void add(BigTorus *res, const BigTorus *a, const BigTorus *b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigTorus_Bench1, add_TT)(benchmark::State &state) {
    for (auto _ : state) {
        tfhe_backend::add(big_torus_arr + 2, big_torus_arr + 1, big_torus_arr + 0, params);
    }
}
BENCHMARK_REGISTER_F(BigTorus_Bench1, add_TT)->Range(1, 128);

static void BigTorusCustomArgs(benchmark::internal::Benchmark* b) {
    const int t[] = {1, 8, 64, 128};
    for (const int nbLimbsFrac: t) {
        for (const int nbLimbsInt: t) {
            if (nbLimbsFrac < nbLimbsInt) continue;
            b->Args({nbLimbsInt, nbLimbsFrac});
        }
    }
}

// void mul(BigTorus *res, const BigInt *a, const BigTorus *b, const ZModuleParams<BigTorus> *params, Allocator alloc);
BENCHMARK_DEFINE_F(BigTorus_Bench2, mul_IT)(benchmark::State &state) {
    Allocator alloc1 = alloc.createStackChildAllocator();
    for (auto _ : state) {
        tfhe_backend::mul(big_torus_arr + 1, big_int_arr + 0, big_torus_arr + 0, params,
                          alloc1.createStackChildAllocator());
    }
}
BENCHMARK_REGISTER_F(BigTorus_Bench2, mul_IT)->Apply(BigTorusCustomArgs);

// void mul(BigTorus *res, int64_t a, const BigTorus *b, const ZModuleParams<BigTorus> *params);
BENCHMARK_DEFINE_F(BigTorus_Bench1, mul_iT)(benchmark::State &state) {
    int64_t a = rand();
    for (auto it : state) {
        tfhe_backend::mul(big_torus_arr + 1, a, big_torus_arr + 0, params);
    }
}
BENCHMARK_REGISTER_F(BigTorus_Bench1, mul_iT)->Range(1, 128);
