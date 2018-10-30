#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

enum GraphInteraction {
  SELECT,
  ADD,
  DELETE
};

class GraphToolbarWidget : public Wt::WContainerWidget
{
public:
  GraphToolbarWidget();

  virtual ~GraphToolbarWidget();

  GraphInteraction currentInteraction() const {
    return current_interaction_;
   }

// private:
//   GraphInteraction current_interaction_;
};


}//namespace view
