#pragma once

#include <Wt/WFont.h>
#include <Wt/WPaintDevice.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPen.h>
#include <Wt/WPainter.h>
#include <Wt/WContainerWidget.h>

#include <graph/Graph.fwd.hh>
#include <graph/Edge.fwd.hh>
#include <graph/Node.fwd.hh>

#include <widgets/color_themes.hh>
#include <widgets/GraphToolbarWidget.hh>

#include <Options.hh>
#include <refresh.hh>

#include <unordered_map>

namespace widgets {

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

class GraphWidget : public Wt::WPaintedWidget, public OptionsHolder, public global_data::Refreshable
{
public:
  GraphWidget(
    graph::GraphSP const &,
    GraphToolbarWidget *
  );

  virtual ~GraphWidget();

  void layoutSizeChanged( int w, int h ) override;

  void setGraph( graph::GraphSP const & );

  graph::GraphSP graph(){ return graph_; }

  void accomodateNewSize( int w, int h );

  void refresh() override {
    update();
  }

protected:
  void init_painting_tools();

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

  GraphInteraction determineInteractionType( Wt::WMouseEvent const & e ) const;

  void mouseDown( Wt::WMouseEvent const & e );
  void fingerDown( Wt::WTouchEvent const & e );
  void down( int x, int y, GraphInteraction );

  void mouseReleased( Wt::WMouseEvent const & e );
  void fingerUp( Wt::WTouchEvent const & e );
  void up( int x, int y, GraphInteraction );

  void mouseDraggedImpl( Wt::WMouseEvent const & e );
  void fingerDrag( Wt::WTouchEvent const & e );
  void drag( int x, int y );

  //TODO this needs a better name
  int getClosestPointForPoint( int point ) const;

  void createPromptForNodeDeletion();
  void createPromptForEdgeDeletion();


private:
  graph::GraphSP graph_;

  ColorThemeCUP theme_;

  GraphToolbarWidget * toolbar_;

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

inline
void
GraphWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}

}//namespace widgets
