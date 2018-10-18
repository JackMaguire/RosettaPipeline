#pragma once

#include <Wt/WContainerWidget>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>

#include <graph/Graph.fwd.hh>
#include <view/color_themes.hh>

namespace view {

class GraphWidget : public Wt::WPaintedWidget
{
public:
  GraphWidget() :
    WPaintedWidget()
  {
    //setLayoutSizeAware( true );
    //resize(200, 60);   // Provide a default size.
  }

  /*void layoutSizeChanged( int w, int h ) override {
    }*/

protected:
  void paintEvent( Wt::WPaintDevice * paintDevice ) override;

private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  int width_;
  int height_;
};

}//namespace view
