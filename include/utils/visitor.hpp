#ifndef VISITOR_H
#define VISITOR_H

#include <variant>

template<class... Ts>
struct overloaded: Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>; // not needed as of C++20

#endif // VISITOR_H
