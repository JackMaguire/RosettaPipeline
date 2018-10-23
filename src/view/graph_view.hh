#pragma once

#include <Wt/WFont>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPen>
#include <Wt/WPainter>
#include <Wt/WContainerWidget>

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
    x_end( x + w ),
    y_end( y + h )
  {}

  int x_begin;
  int y_begin;
  int x_end;
  int y_end;

  bool pointIsInBox( int x, int y ) const {
    return ( x >= x_begin ) && ( x < x_end ) && ( y >= y_begin )
      && ( y < y_end );
  }
};

class GraphWidget : public Wt::WPaintedWidget
{
public:
  GraphWidget( Wt::WContainerWidget *root );

  GraphWidget( Wt::WContainerWidget *root, graph::GraphSP const & );

  virtual ~GraphWidget();

  void layoutSizeChanged( int w, int h ) override;

  void setGraph( graph::GraphSP const & );

protected:
  void paintEvent( Wt::WPaintDevice * paintDevice ) override;

  void
  drawNode(
    graph::NodeSP const &,
    Wt::WPainter &,
    int const grid_size,
    int const selection_width
  );

  void
  drawEdge(
    graph::EdgeSP const &,
    Wt::WPainter &,
    int const grid_size,
    int const offset
  );

  void
  drawGhostEdge(
    graph::PreliminaryEdgeCSP const & ghost_edge,
    Wt::WPainter &,
    int const grid_size,
    int const offset
  );

protected:
  void init_listeners();

  void mouseDown( Wt::WMouseEvent const & e );

  int getClosestPointForPoint( int point ) const;

private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  int width_;
  int height_;

  int node_width_ = 3;

  Wt::WPen default_pen_;
  Wt::WPen selected_edge_pen_;

  Wt::WFont node_label_font_;

  std::unordered_map< graph::NodeSP, hitbox > hitbox_for_node_;
  std::unordered_map< graph::EdgeSP, hitbox > hitbox_for_edge_;


  //Controller
  int last_mouse_press_x_;
  int last_mouse_press_y_;
  bool node_is_currently_being_dragged_;
  bool edge_is_currently_being_created_;
  bool shift_was_down_when_most_recent_object_was_selected_;
};

inline
void
GraphWidget::setGraph( graph::GraphSP const & graph ) {
  graph_ = graph;
}

}//namespace view
