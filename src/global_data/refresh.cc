#include <global_data/refresh.hh>

#include <vector>
#include <algorithm>

#include <iostream>

namespace global_data {

thread_local std::vector< Refreshable * > objects;

void refresh_all_objects() {
  std::cout << "Refreshing " << objects.size() << " elements" << std::endl;
  for( Refreshable * obj : objects ){
    obj->refresh();
  }
}

void register_refreshable_object( Refreshable * obj ){
  std::cout << "Adding element " << obj << std::endl;
  if( obj != 0 ){
    std::cout << "Size = " << objects.size() << std::endl;
    objects.push_back( obj );
  }
}

void deregister_refreshable_object( Refreshable * obj ){
  std::cout << "Removing element " << obj << std::endl;
  auto iter = std::find( objects.begin(), objects.end(), obj );
  if( iter != objects.end() ){
    objects.erase( iter );
  }
}


}
