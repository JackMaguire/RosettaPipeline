#pragma once

#include <vector>
#include <algorithm>

enum ListenType {
  EXTRA_FILE_EDIT,
  EXTRA_FILE_LIST
};

//Allows classes to derive from more than one Listener/Changable without impacting the vtable
template< ListenType T >
class Identifier{};

namespace listeners {

template< ListenType T >
class Listener {
public:
  virtual void register_change( Identifier< T > ) = 0;
};

template< ListenType T >
class Changable {
public:
  void add_listener( Listener< T > * listener ){
    if( listener ){
      listeners_.push_back( listener );
    }
  }

  void remove_listener( Listener< T > * listener ){
    auto iter = std::find( listeners_.begin(), listeners_.end(), listener );
    if( iter != listeners_.end() ){
      listeners_.erase( iter );
    }
  }

protected:
  void announce_change( Identifier< T > i ){
    for( Listener< T > * l : listeners_ ){
      l->register_change( i );
    }
  }

private:
  std::vector< Listener< T > * > listeners_;
};

}
