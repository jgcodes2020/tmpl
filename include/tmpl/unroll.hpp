#ifndef _TMPL_UNROLL_HPP_
#define _TMPL_UNROLL_HPP_

#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201703L
namespace tmpl {
  namespace details {
    template <class T, T... Is, class F>
    constexpr void unroll_loop(std::integer_sequence<T, Is...>, F&& f) {
      (void(f(Is)), ...);
    }
  }  // namespace details
  template <class T, T max, class F>
  constexpr void unroll_loop(F&& f) {
    details::unroll_loop(std::make_integer_sequence<T, max> {}, f);
  }
}  // namespace tmpl
#elif __cplusplus >= 201103L
namespace tmpl {
  namespace details {
    #if __cplusplus < 201402L
    template <class T, T... values>
    class integer_sequence {
    public:
      static constexpr size_t size() { return sizeof...(values); }
    };

    template <class T, class N, class... Is>
    struct make_integer_sequence_helper
        : make_integer_sequence_helper<
            T, std::integral_constant<T, N::value - 1>,
            std::integral_constant<T, N::value - 1>, Is...> {};

    template <class T, class... Is>
    struct make_integer_sequence_helper<
      T, std::integral_constant<T, 0>, Is...> {
      using type = integer_sequence<T, Is::value...>;
    };

    template <class T, T N>
    using make_integer_sequence = typename make_integer_sequence_helper<
      T, std::integral_constant<T, N>>::type;
    #else
    using std::integer_sequence;
    using std::make_integer_sequence;
    #endif
    
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
  }  // namespace details
  
  /**
   * @brief Unrolls the loop code in the lambda function provided.
   * 
   * @tparam T the type to pass to the iteration function
   * @tparam max the number to iterate up to
   * @param f the function to iterate
   */
  template <class T, T max, class F>
  void unroll_loop(F&& f) {
    details::unroll_loop(details::make_integer_sequence<T, max> {}, f);
  }
}  // namespace tmpl
#endif
#endif