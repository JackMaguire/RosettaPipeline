#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>

#include <view/graph_view.hh>

namespace view {

class TopWidget : public Wt::WContainerWidget
{
public:
  TopWidget( Wt::WContainerWidget *root, graph::GraphSP const & );

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
