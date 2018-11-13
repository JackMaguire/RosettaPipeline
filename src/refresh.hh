#pragma once

#include <vector>
#include <memory>

class Refreshable {
public:
  virtual void refresh() = 0;
};

struct RefreshableElementVec {
  
  void refresh_all_objects(){
    for( Refreshable * obj : objects ){
      obj->refresh();
    }
  }

  void push_back( Refreshable * element ){
    elements.push_back( element );
  }

  std::vector< Refreshable * > elements;

};

using RefreshableElementVecSP = std::shared_ptr< RefreshableElementVec >;

