#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>
#include <iostream>
#include <view/GraphWidget.hh>


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

  GraphWidget * graph_widget_;
};

inline
void
TopWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;

  if( graph_widget_ ){
    graph_widget_->accomodateNewSize( w / 3, h / 2 );
  }
}


}//namespace view
