#pragma once

namespace misc {

//from https://stackoverflow.com/questions/43526647/decltype-of-function-parameter
template <typename> struct FirstArgument;

template <typename R, typename A, typename... Args>
struct FirstArgument<R(A, Args...)>
{
   using type = A;
};

template <typename T>
using first_agument_t = typename FirstArgument<T>::type;

/*
void foo(int a){ }

void bar(int a, double b){ }

int main()
{
   long value = 1L;
   foo(static_cast<first_agument_t<decltype(foo)>>(value) );
   bar(static_cast<first_agument_t<decltype(bar)>>(value), 0);
}
*/

}
