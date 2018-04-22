#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <core/allocator/allocator.h>
#include <core/arithmetic/random_gen.h>

using namespace std;
using namespace tfhe_backend;
using namespace std;

TEST(RandomGen, DISABLED_set_seed) { // see why seeding doest not work
    double v1, v2;

    // obtain same value with same seed
    RandomGen::set_seed(42);
    v1 = RandomGen::gaussian(1);
    RandomGen::set_seed(42);
    v2 = RandomGen::gaussian(1);

    EXPECT_DOUBLE_EQ(v1, v2);
}

TEST(RandomGen, uniform_bool) {
    const int nb_iter = 100;

    vector<int> val_cnt = {0, 0};
    for (int i = 0; i < nb_iter; ++i) {
        auto val = RandomGen::uniform_bool();
        ASSERT_GE(val, 0);
        ASSERT_LE(val, 1);
        val_cnt.at(val)++;
    }

    EXPECT_GT(val_cnt[0], 0);
    EXPECT_GT(val_cnt[1], 0);
}

TEST(RandomGen, gaussian) {
    const int nb_iter = 100;

    vector<double> vals;
    for (int i = 0; i < nb_iter; ++i)
        vals.push_back(RandomGen::gaussian(1));

    // -5.sigma<x<5.sigma with overhelming probability
    double max_val = *max_element(vals.begin(), vals.end());
    EXPECT_LE(max_val, 5);
    double min_val = *min_element(vals.begin(), vals.end());
    EXPECT_GE(min_val, -5);

    EXPECT_GE(max_val-min_val, 2);

    double sum_val = accumulate(vals.begin(), vals.end(), 0.0);
    double mean_val = sum_val / nb_iter;
    EXPECT_LE(mean_val, 0.4);
    EXPECT_GE(mean_val, -0.4);
}

template <typename TORUS>
class RandomGenTorusTestBase : public ::testing::Test {
public:
    Allocator alloc;
    ZModuleParams<TORUS> *params = nullptr;

    const int sample_cnt = 2;
    TORUS *sample_arr = nullptr;
};

template <typename TORUS>
class RandomGenTorusTest : public RandomGenTorusTestBase<TORUS> {
public:
    Allocator alloc;

    void SetUp() {
        this->sample_arr = this->alloc.template newArray<TORUS>(this->sample_cnt);
    }

    void TearDown() {
        this->alloc.deleteArray(this->sample_cnt, this->sample_arr);
    }
};

template<>
class RandomGenTorusTest<BigTorus> : public RandomGenTorusTestBase<BigTorus> {
public:
    void SetUp() {
        params = alloc.newObject<ZModuleParams<BigTorus>>(5);
        sample_arr = alloc.newArray<BigTorus>(sample_cnt, params, &alloc);
    }

    void TearDown() {
        alloc.deleteArray(sample_cnt, sample_arr, params, &alloc);
        alloc.deleteObject(params);
    }
};


typedef ::testing::Types<Torus32, Torus64, BigTorus> TorusTypes;
TYPED_TEST_CASE(RandomGenTorusTest, TorusTypes);

TYPED_TEST(RandomGenTorusTest, uniform) {
    typedef TypeParam TORUS;

    const int nb_iter = 10;

    vector<double> vals;
    for (int i = 0; i < nb_iter; ++i) {
        RandomGenTorus<TORUS>::uniform(this->sample_arr[0], this->params);
        double sampled = TorusUtils<TORUS>::to_double(this->sample_arr[0], this->params);
        vals.push_back(sampled);
    }

    double min_val = *min_element(vals.begin(), vals.end());
    double max_val = *max_element(vals.begin(), vals.end());
    ASSERT_NE(min_val, max_val);
}

TYPED_TEST(RandomGenTorusTest, gaussian) {
    typedef TypeParam TORUS;

    const int nb_iter = 1;

    vector<double> vals;
    for (int i = 0; i < nb_iter; ++i) {
        TORUS &sample = this->sample_arr[0];
        TORUS &mean = this->sample_arr[1];

        double sigma = (double)rand() / RAND_MAX / 100;
        RandomGenTorus<TORUS>::gaussian(sample, mean, sigma, this->params);

        // -5.sigma<x<5.sigma with overhelming probability
        double dist = TorusUtils<TORUS>::normInftyDist(sample, mean, this->params);
        EXPECT_LE(dist, 5 * sigma);
    }
}
