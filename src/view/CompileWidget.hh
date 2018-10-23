#pragma once

#include <Wt/WTabWidget>
#include <graph/Graph.fwd.hh>

namespace view {

class CompileWidget : public Wt::WTabWidget
{
public:
  CompileWidget( graph::GraphSP const & );

  virtual ~CompileWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;
};

inline
void
CompileWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
