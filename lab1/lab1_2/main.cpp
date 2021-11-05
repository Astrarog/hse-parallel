#include <iostream>
#include <functional>
#include <sstream>
#include <chrono>

using namespace std;

double trapezoidalIntegral(double a, double b, int n, const std::function<double (double)> &f) {
    const double width = (b-a)/n;

    double trapezoidal_integral = 0;
    for(size_t step = 0; step < n; ++step) {
        const double x1 = a + step*width;
        const double x2 = a + (step+1)*width;

        trapezoidal_integral += 0.5*(x2-x1)*(f(x1) + f(x2));
    }

    return trapezoidal_integral;
}

size_t parse_number(char * arg){
    istringstream str(arg);
    size_t n;
    str >> n;

    if(!str)
        throw std::invalid_argument("Provided number is incorrect");

    return n;
}

int main(int arc, char* argv[])
{
    using time_t = std::chrono::duration<double, std::milli>;

    size_t a = parse_number(argv[1]);
    size_t b = parse_number(argv[2]);
    size_t n = parse_number(argv[3]);

    auto f = [](double x){ return x*x*x + 3./x + 1; };

    auto start = chrono::steady_clock::now();

    double result = trapezoidalIntegral(a, b, n, f);

    auto end = chrono::steady_clock::now();

    time_t time = (end-start);

    cout << "result=" << result << " time=" << time.count() << endl;

    return 0;
}
