#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <chrono>
#include <tuple>

#include <immintrin.h>

using namespace std;

namespace hse {

vector<double> generate_random(size_t n){

    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.5, 1.605);

    vector<double> data;
    data.reserve(n);

    for (size_t i=0; i<n; ++i){
        data.push_back(dis(gen));
    }

    return data;
}

size_t parse_n(char * arg1){
    istringstream str(arg1);
    size_t n;
    str >> n;

    if(!str)
        throw std::invalid_argument("Provided data size is incorrect");

    return n;
}

using time_t = std::chrono::duration<double, std::milli>;

}

tuple<double, hse::time_t> measure_multipy_single(const vector<double>& x,
                                                  const vector<double>& y){

    double result = 1.;

    auto start = chrono::steady_clock::now();

    for(size_t i=0; i< x.size(); ++i){
        result *= (x[i] * y[i] );
    }

    auto end = chrono::steady_clock::now();

    return {result, (end-start)};

}

tuple<double, hse::time_t> measure_multipy_vector(const vector<double>& x,
                                                  const vector<double>& y){

    const __m128d *x_vec = reinterpret_cast<const __m128d *>(x.data()),
                  *y_vec = reinterpret_cast<const __m128d *>(y.data());

    __m128d p = _mm_set1_pd(1.);

    constexpr size_t v_len = (128/8/sizeof(double));

    // start
    auto start = chrono::steady_clock::now();

    for(int i=0; i< x.size()/v_len; ++i){
        __m128d tmp = _mm_mul_pd(x_vec[i], y_vec[i]);
        p = _mm_mul_pd(p, tmp);
    }

    double result = 1.;
    for (size_t i=0; i<v_len; ++i)
        result *= (reinterpret_cast<double*>(&p))[i];

    //end
    auto end = chrono::steady_clock::now();

    return {result, end-start};

}


int main(int arc, char* argv[])
{
    size_t n = hse::parse_n(argv[1]);
    vector<double> x = hse::generate_random(n);
    vector<double> y = hse::generate_random(n);

    auto [result_s, time_s] = measure_multipy_single(x, y);
    auto [result_v, time_v] = measure_multipy_vector(x, y);

    cout << "result" "\t" "n" "\t" "latency" "\t" "IPS" << endl;
    cout << result_s << "\t" << n << "\t" << time_s.count() << "\t" << (n / time_s.count()) << endl;
    cout << result_v << "\t" << n << "\t" << time_v.count() << "\t" << (n / time_v.count()) << endl;
    return 0;
}
