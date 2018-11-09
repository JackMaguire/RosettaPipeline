#include <global_data/refresh.hh>

#include <vector>
#include <algorithm>

namespace global_data {

thread_local std::vector< Refreshable * > objects;

void refresh_all_objects() {
  for( Refreshable * obj : objects ){
    obj->refresh();
  }
}

void register_refreshable_object( Refreshable * obj ){
  if( obj != 0 ){
    objects.push_back( obj );
  }
}

void deregister_refreshable_object( Refreshable * obj ){
  auto iter = std::find( objects.begin(), objects.end(), obj );
  if( iter != objects.end() ){
    objects.erase( iter );
  }
}


}
