# Template Meta-Programming Library
Some useful things involving template meta-programming.  
They are explained below.

## Template loop unroll (`tmpl/unroll.hpp`)
Firstly, C++11 lacks `integer_sequence` and `make_integer_sequence`, so I wrote (read: stole from StackOverflow) some boilerplate code to implement those.

The real magic happens in these three functions:
```c++
inline void init_list_noop(std::initializer_list<int>) {}

template<class T, T val, class F>
int call_and_return_0(F&& f) {
  f(val);
  return 0;
}

template<class T, T... vals, class F>
void unroll_loop(integer_sequence<T, vals...>, F&& f) {
  init_list_noop({call_and_return_0<T, vals>(f)...});
}
```
The key reason this works (reliably, of course), is because of the braced list. Items in a braced initializer list are evaluated from left to right, and this is even guaranteed with pack expansion. 

However, for this pack expansion to work, the expansion pattern has to return something. So, this is where `call_and_return_0()` works its magic. It simply invokes the function and returns 0, which can be inserted into the parameter pack.

Finally, because the elements must be evaluated from left to right, the above pack expansion guarantees that the parameter pack is iterated in order.

### C++17 version
```c++
template <class T, T... Is, class F>
constexpr void unroll_loop(std::integer_sequence<T, Is...>, F&& f) {
  (f(Is), ...);
}
```
C++17 *fold-expressions* allow us to expand a parameter pack using a binary operator. This trivializes the problem: we can use the comma operator to execute multiple statements one after the other, and as such we can use it to invoke the function in order.

Obviously this implementation will break if you overload `operator,()`. So I fixed *that* by casting all function results to void, just in case.