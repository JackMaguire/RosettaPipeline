#pragma once

#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPen>
#include <Wt/WPainter>

#include <graph/Graph.fwd.hh>
#include <graph/Edge.fwd.hh>
#include <graph/Node.fwd.hh>

#include <view/color_themes.hh>

#include <memory>

namespace view {

class GraphWidget : public Wt::WPaintedWidget
{
public:
  GraphWidget();

  virtual ~GraphWidget();

  void layoutSizeChanged( int w, int h ) override;

protected:
  void paintEvent( Wt::WPaintDevice * paintDevice ) override;

  void
  drawEdge(
    graph::EdgeSP const & edge,
    Wt::WPainter const & painter,
    int const grid_size,
    int const offset
  );

private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  int width_;
  int height_;

  int node_width_ = 3;

  std::shared_ptr< Wt::WPen > default_pen_;
  std::shared_ptr< Wt::WPen > selected_edge_pen_;
};

}//namespace view
