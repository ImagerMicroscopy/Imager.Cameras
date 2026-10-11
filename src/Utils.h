#ifndef UTILS_H
#define UTILS_H

#include <functional>

class Finally {
public:
    Finally(std::function<void()> f) : f_(f) {}
    ~Finally() {f_();}

    Finally(const Finally&) = delete;
    Finally& operator= (const Finally&) = delete;

private:
    std::function<void()> f_;
};

#endif
