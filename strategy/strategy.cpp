// Idea; fibonacci where one is DP, the other is recursion
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

double get_time_diff(struct timespec* start, struct timespec* end) {
    return (end->tv_sec - start->tv_sec) + 1e-9 * (end->tv_nsec - start->tv_nsec);
}

struct timespec start, end;

class FibonacciStrategy {
public:
    FibonacciStrategy() {}
    virtual std::string name() = 0;
    virtual ~FibonacciStrategy() {}
    virtual int fibonacci(int n) = 0;
};

class Recursive: public FibonacciStrategy {
    std::string name() {
        return "Recursive";
    }

    int fibonacci(int n) {
        if (n <= 0) {
            return 0;
        }
        if (n == 1) {
            return 1;
        }
        else {
            return fibonacci(n - 1) + fibonacci(n - 2);
        }
    }
};

class DynamicProgramming: public FibonacciStrategy {
    std::vector<int> f_arr;
public:
    std::string name() {
        return "Dynamic Programming";
    }

    int fibonacci(int n) {
        int old_size = f_arr.size();
        if ((n + 1) > old_size) {
            int new_size = std::max<int>(n + 1, 2 * old_size);
            f_arr.resize(new_size);
            for (int index = old_size; index < new_size; index++) {
                if (index <= 0) {
                    f_arr[index] = 0;
                }
                else if (index == 1) {
                    f_arr[index] = 1;
                }
                else {
                    f_arr[index] = f_arr[index - 1] + f_arr[index - 2];
                }
            }
        }
        return f_arr[n];
    }
};

class Summation: public FibonacciStrategy {
public:
    std::string name() {
        return "Summation";
    }

    int fibonacci(int n) {
        if (n <= 0) {
            return 0;
        }
        if (n == 1) {
            return 1;
        }
        else {
            int* f_arr = (int*) malloc(sizeof(int) * (n + 1));
            for (int i = 0; i < (n + 1); i++) {
                if (i == 0 || i == 1) {
                    f_arr[i] = i;
                }
                else {
                    f_arr[i] = f_arr[i - 1] + f_arr[i - 2];
                }
                // std::cout << "f_arr[" << i << "]: " << f_arr[i] << " ";
            }
            int result =  f_arr[n];
            free(f_arr);
            // std::cout << std::endl << "Result: " << f_arr[n + 1] << std::endl;
            return result;
        }
    }
};

class FibApp {
private:
    class FibonacciStrategy* strategy;
public:
    FibApp() {}
    void chooseStrategy(FibonacciStrategy* strategy) {
        this->strategy = strategy;
    }
    int doCalculation(int i) {
        int r = this->strategy->fibonacci(i);
        return r;
    }

    void time(int num_trials) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i = 0; i < num_trials; i++) {
            doCalculation(i);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);
        std::cout << strategy->name() << " time: " << get_time_diff(&start, &end) << " seconds" << std::endl;
    }
};

int main() {
    FibApp app;
    Summation s;
    Recursive r;
    DynamicProgramming dp;

    int num_trials = 40;

    
    app.chooseStrategy(&s);
    app.time(num_trials);

    app.chooseStrategy(&r);
    app.time(num_trials);

    app.chooseStrategy(&dp);
    app.time(num_trials);
}