#pragma once

#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPen>
#include <Wt/WPainter>

#include <graph/Graph.fwd.hh>
#include <graph/Edge.fwd.hh>
#include <graph/Node.fwd.hh>

#include <view/color_themes.hh>
#include <unordered_map>

namespace view {

struct hitbox {
  hitbox(){}

  hitbox( int x, int y, int w, int h ):
    x_begin( x ),
    y_begin( y ),
    width( w ),
    height( h )
  {}

  int x_begin;
  int y_begin;
  int width;
  int height;
};

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
    Wt::WPainter & painter,
    int const grid_size,
    int const offset
  );

  void
  drawGhostEdge(
    graph::PreliminaryEdgeCSP const & ghost_edge,
    Wt::WPainter & painter,
    int const grid_size,
    int const offset
  );

private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  int width_;
  int height_;

  int node_width_ = 3;

  Wt::WPen default_pen_;
  Wt::WPen selected_edge_pen_;

  std::unordered_map< graph::NodeSP, hitbox > hitbox_for_node_;
  std::unordered_map< graph::EdgeSP, hitbox > hitbox_for_edge_;
};

}//namespace view
