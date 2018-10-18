#pragma once

#ifndef AAA
#define AAA

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
  GraphWidget();

  virtual ~GraphWidget();

  /*void layoutSizeChanged( int w, int h ) override {
    }*/

protected:
  void paintEvent( Wt::WPaintDevice * paintDevice ) override;

private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  int my_width_;
  int my_height_;
};

}//namespace view

#endif
