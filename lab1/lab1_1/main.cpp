#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <chrono>
#include <tuple>

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

tuple<double, hse::time_t> measure_multipy_single(const vector<double>& data){

    double result = 1.;

    auto start = chrono::steady_clock::now();

    for(size_t i=0; i<data.size(); ++i){
        result *= data[i];
    }

    auto end = chrono::steady_clock::now();

    return {result, (end-start)};

}

void measure_multipy_vector(){

}


int main(int arc, char* argv[])
{
    size_t n = hse::parse_n(argv[1]);
    vector<double> data = hse::generate_random(n);

    auto [result, time] = measure_multipy_single(data);

    cout << "result" "\t" "n" "\t" "latency" "\t" "IPS" << endl;
    cout << result << "\t" << data.size() << "\t" << time.count() << "\t" << (data.size() / time.count()) << endl;
    return 0;
}
