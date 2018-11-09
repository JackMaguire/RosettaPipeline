#pragma once

namespace global_data {

class Refreshable {
public:
  virtual void refresh() = 0;
};

void refresh_all_objects();

void register_refreshable_object( Refreshable * );

void deregister_refreshable_object( Refreshable * );

}
