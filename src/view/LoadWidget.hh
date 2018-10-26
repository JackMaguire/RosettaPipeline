#pragma once

//#include <Wt/WTabWidget.h>
#include <Wt/WFileUpload.h>
#include <graph/Graph.fwd.hh>

namespace view {

class LoadWidget : public Wt::WFileUpload
{
public:
  LoadWidget( graph::GraphSP const & );

  virtual ~LoadWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;
};

inline
void
LoadWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
