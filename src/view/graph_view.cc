#include <view/graph_view.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt/WLength>
#include <Wt/WGlobal>

#include <math.h>

namespace view {

GraphWidget::GraphWidget( Wt::WContainerWidget * root ) :
  WPaintedWidget( root ),
  graph_( 0 ),
  theme_( std::make_unique< Theme1 >() ),
  width_( 1000 ),
  height_( 800 )
{
  //width_ = width().toPixels();
  //height_ = height().toPixels();

  //TODO
  //node_label_font_.setFamily( Wt::FontFamily::Monospace, "'Courier New'" );

  setLayoutSizeAware( true );

  resize( width_, height_ ); // Provide a default size.
}

GraphWidget::GraphWidget(
  Wt::WContainerWidget * root,
  graph::GraphSP const & graph
) :
  WPaintedWidget( root ),
  graph_( graph ),
  theme_( std::make_unique< Theme1 >() ),
  width_( 1000 ),
  height_( 800 )
{
  //width_ = width().toPixels();
  //height_ = height().toPixels();

  //TODO
  //node_label_font_.setFamily( Wt::FontFamily::Monospace, "'Courier New'" );

  setLayoutSizeAware( true );

  resize( width_, height_ ); // Provide a default size.
}

GraphWidget::~GraphWidget(){}

void
GraphWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}

void
GraphWidget::paintEvent( Wt::WPaintDevice * paintDevice ) {
  Wt::WPainter painter( paintDevice );
  painter.setPen( default_pen_ );

  //Background
  //
  painter.setBrush( Wt::WBrush( theme_->background() ) );
  painter.drawRect( 0, 0, width_, height_ );

  //Grid
  //
  painter.setBrush( Wt::WBrush( theme_->grid_line() ) );
  int const grid_size = global_data::Options::grid_size;
  for( int x = -1; x < width_; x += 2 * grid_size ) {
    painter.drawLine( x, 0, x, height_ );
  }
  for( int y = -1; y < height_; y += 2 * grid_size ) {
    painter.drawLine( 0, y, width_, y );
  }

  //Edges
  //
  painter.setBrush( Wt::WBrush( theme_->edge() ) );
  int const edge_offset = ( node_width_ - ( node_width_ / 2 ) ) * grid_size;
  for( auto const & edge_sp : graph_->edges() ){
    drawEdge( edge_sp, painter, grid_size, edge_offset );
  }
  painter.setPen( default_pen_ );

  //Ghost Edge
  //
  graph::PreliminaryEdgeCSP ghost_edge = graph_->ghostEdge();
  if( ghost_edge ){
    drawGhostEdge( ghost_edge, painter, grid_size, edge_offset );
  }

  //Nodes
  //
  node_label_font_.setSize( Wt::WLength( 3 * grid_size ) );
  painter.setFont( node_label_font_ );
  int const selection_width = grid_size / 2;
  for( auto const & node_sp : graph_->nodes() ){
    drawNode( node_sp, painter, grid_size, selection_width );
  }
}

void
GraphWidget::drawNode(
  graph::NodeSP const & node,
  Wt::WPainter & painter,
  int const grid_size,
  int const selection_width
) {
  int const x = node->X() * grid_size + ( grid_size / 2 );
  int const y = node->Y() * grid_size + ( grid_size / 2 );
  int const diameter = grid_size * 3;
  
  if( node == graph_->selectedNode() ){
    painter.setBrush( Wt::WBrush( theme_->selection_outline() ) );
    int const sx = x - selection_width;
    int const sy = y - selection_width;
    int const sdiameter = diameter + 2 * selection_width;
    painter.drawEllipse( sx, sy, sdiameter, sdiameter );
  }

  if( node->numDownstreamEdges() == 0 ) {
    painter.setBrush( Wt::WBrush( theme_->final_node() ) );
  } else {
    painter.setBrush( Wt::WBrush( theme_->intermediate_node() ) );
  }

  painter.drawEllipse( x, y, diameter, diameter );
  hitbox_for_node_[ node ] = hitbox( x, y, diameter, diameter );

  if( global_data::Options::show_node_titles ) {
    //g2D.setColor( Color.black );
    if( global_data::Options::put_node_titles_to_side ) {
      painter.drawText(
	x + diameter + selection_width,
	y,
	100,//width
	100,//height
	Wt::WFlags< Wt::AlignmentFlag >(),
	node->title()
      );
    } else {
      painter.drawText(
	x,
	y - selection_width,
	100,//width
	100,//height
	Wt::WFlags< Wt::AlignmentFlag >(),
	node->title()
      );
    }
  }
}



void
GraphWidget::drawEdge(
  graph::EdgeSP const & edge,
  Wt::WPainter & painter,
  int const grid_size,
  int const offset
) {
  graph::Node const & n_from = edge->sourceNode();
  graph::Node const & n_to = edge->destinationNode();

  if( edge == graph_->selectedEdge() ) {
    painter.setPen( selected_edge_pen_ );
  } else {
    painter.setPen( default_pen_ );
  }

  // Draw main line
  int const source_x = n_from.X() * grid_size + offset;
  int const source_y = n_from.Y() * grid_size + offset;
  int const dest_x = n_to.X() * grid_size + offset;
  int const dest_y = n_to.Y() * grid_size + offset;
  painter.drawLine( source_x, source_y, dest_x, dest_y );

  // Draw Arrow Heads
  // B is halfway point between two nodes
  double const Bx = grid_size * ( n_to.X() + n_from.X() ) / 2 + offset;
  double const By = grid_size * ( n_to.Y() + n_from.Y() ) / 2 + offset;
  double const arrow_length = grid_size;

  double const theta_in_radians = atan2( double(source_y) - By, double(source_x) - Bx );

  {//copied from https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line
    int const x1 = (int) Bx;
    int const y1 = (int) By;
    int const x2 = int( Bx + arrow_length );
    int const y2 = int( By - arrow_length );
    int const dx = x2-x1;
    int const dy = y2-y1;
    int const nx = dx * cos( theta_in_radians ) - dy * sin( theta_in_radians ) + x1;
    int const ny = dx * sin( theta_in_radians ) + dy * cos( theta_in_radians ) + y1;
    painter.drawLine( x1, y1, nx, ny );
  }

  {//copied from https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line
    int const x1 = (int) Bx;
    int const y1 = (int) By;
    int const x2 = int( Bx + arrow_length );
    int const y2 = int( By + arrow_length );
    int const dx = x2-x1;
    int const dy = y2-y1;
    int const nx = dx * cos( theta_in_radians ) - dy * sin( theta_in_radians ) + x1;
    int const ny = dx * sin( theta_in_radians ) + dy * cos( theta_in_radians ) + y1;
    painter.drawLine( x1, y1, nx, ny );
  }

  hitbox_for_edge_[ edge ] = hitbox(
    (int) ( Bx - grid_size ),
    (int) ( By - grid_size ),
    grid_size * 2,
    grid_size * 2
  );

}

void
GraphWidget::drawGhostEdge(
  graph::PreliminaryEdgeCSP const & ghost_edge,
  Wt::WPainter & painter,
  int const grid_size,
  int const offset
) {
  graph::NodeSP const & n_from = ghost_edge->source_node;
  int const source_x = n_from->X() * grid_size + offset;
  int const source_y = n_from->Y() * grid_size + offset;
  painter.drawLine( source_x, source_y, ghost_edge->cursor_x, ghost_edge->cursor_y );
}

}//namespace view
