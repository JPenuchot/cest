#ifndef _CE_FORWARD_LIST_HPP_
#define _CE_FORWARD_LIST_HPP_

// $MYGCC/bin/g++ -std=c++2a -I .. -c ../../tests/forward_list_tests.hpp

#include <memory>

namespace cestd {

template <
  class T,
  class Allocator = std::allocator<T>
>
class forward_list {
public:
  struct node;
  struct iter;

  using value_type      = T;
  using allocator_type  = Allocator;
  using reference       =       value_type&;
  using const_reference = const value_type&;
  using iterator        =       iter;
  using const_iterator  = const iter;
  using size_type       = std::size_t;

  struct iter {
    constexpr reference operator*()  { return curr_node->value;  }
    constexpr iter&     operator++()    {        // pre-increment
      curr_node = curr_node->next_node;
      return *this;
    }
    constexpr iter      operator++(int) {        // post-increment
      iter tmp(curr_node);
      ++(*this);
      return tmp; 
    }
    constexpr bool      operator==(const iter &other) {
      return this->curr_node == other.curr_node;
    }
    node *curr_node;
  };

  struct node {
    value_type value;
    node      *next_node;
  };

  constexpr  forward_list() : m_front(nullptr) {}
  constexpr ~forward_list() {
    node *curr_node = m_front;
    while (curr_node) {
      node *next_node = curr_node->next_node;
      std::destroy_at(curr_node);
      m_node_alloc.deallocate(curr_node,1);
      curr_node = next_node;
    };
  }
  constexpr       iterator  begin()       noexcept { return iter(m_front); }
  constexpr const_iterator  begin() const noexcept { return iter(m_front); }
  constexpr const_iterator cbegin() const noexcept { return iter(m_front); }
  constexpr       iterator    end()       noexcept { return iter(nullptr); }
  constexpr const_iterator    end() const noexcept { return iter(nullptr); }
  constexpr const_iterator   cend() const noexcept { return iter(nullptr); }
  constexpr void push_front(const T &value) {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node,value,m_front);
    m_front = new_node;
  }
  constexpr void push_front(T &&value)      {
    node *new_node = m_node_alloc.allocate(1);
    std::construct_at(new_node,std::forward<T>(value),m_front);
    m_front = new_node;
  }
  constexpr reference       front()         { return m_front->value; }
  constexpr const_reference front() const   { return m_front->value; }

  node *m_front;
  allocator_type m_alloc;
  std::allocator_traits<allocator_type>::template rebind_alloc<node> m_node_alloc;
};

} // namespace cestd

#endif // _CE_FORWARD_LIST_HPP_