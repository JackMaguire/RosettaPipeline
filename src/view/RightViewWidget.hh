#pragma once

#include <Wt/WTabWidget>
#include <graph/Graph.fwd.hh>

namespace view {

class RightViewWidget : public Wt::WTabWidget
{
public:
  RightViewWidget( Wt::WContainerWidget *root, graph::GraphSP const & );

  virtual ~RightViewWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;
};

inline
void
RightViewWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
