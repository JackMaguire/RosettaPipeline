#pragma once

#include <Wt/WTabWidget.h>
#include <graph/Graph.fwd.hh>

namespace view {

class EditWidget : public Wt::WTabWidget
{
public:
  EditWidget( graph::GraphSP );

  virtual ~EditWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  graph::GraphSP graph_;
  Wt::WContainerWidget * current_;
  int width_;
  int height_;
};

inline
void
EditWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
