#pragma once

#include <Wt/WTabWidget>
#include <graph/Graph.fwd.hh>

namespace view {

class EditWidget : public Wt::WTabWidget
{
public:
  EditWidget( graph::GraphSP const & );

  virtual ~EditWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
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
