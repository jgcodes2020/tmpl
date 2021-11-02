#ifndef _TMPL_OVERLOAD_HPP_
#define _TMPL_OVERLOAD_HPP_

#include <utility>

#if __cplusplus >= 201703L
namespace tmpl {
  template<class... Ts>
  struct overload : Ts... {
    using Ts::operator()...;
  };
  template<class... Ts>
  overload(Ts...) -> overload<Ts...>;
}
#elif __cplusplus >= 201103L
namespace tmpl {
  template<class T0, class... Ts>
  struct overload : T0, overload<Ts...> {
    overload(T0&& f0, Ts&&... fs) : T0(f0), overload<Ts...>(fs...) {}
    
    using T0::operator();
    using overload<Ts...>::operator();
  };
  
  template<class T0>
  struct overload<T0> {
    overload(T0&& f0) : T0(f0) {}
    
    using T0::operator();
  };
}
#endif
namespace tmpl {
  template<typename... Ts>
  overload<Ts...> make_overload(Ts&&... fs) {
    return overload<Ts...>(std::forward(fs...));
  }
}


#endif