#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>

namespace view {

class TopWidget : public Wt::WContainerWidget
{
public:
  TopWidget(
    graph::GraphSP,
    OptionsSP
  );

  virtual ~TopWidget();

  void layoutSizeChanged( int w, int h ) override;

private:
  int width_;
  int height_;
};

inline
void
TopWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
