#include <view/graph_view.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt/WLength>

namespace view {

GraphWidget::GraphWidget() :
  WPaintedWidget(),
  graph_( 0 ),
  theme_(),
  width_( 200 ),
  height_( 20 )
{
  width_ = width().toPixels();
  height_ = height().toPixels();

  setLayoutSizeAware( true );

  resize( Wt::WLength::Auto, Wt::WLength::Auto ); // Provide a default size.
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
}

void
GraphWidget::drawEdge(
  graph::EdgeSP const & edge,
  Wt::WPainter const & painter,
  int const grid_size,
  int const offset
) const {
  graph::Node const & n_from = edge->sourceNode();
  graph::Node const & n_to = edge->destinationNode();

  if( edge == graph_->selectedEdge() ) {
    painter.setPen( selected_edge_pen_ );
  } else {
    painter.setPen( default_pen_ );
  }

  // Draw main line
  int const source_x = n_from.X() * grid_size + offset;
  int const source_y = n_from.X() * grid_size + offset;
  int const dest_x = n_to.X() * grid_size + offset;
  int const dest_y = n_to.X() * grid_size + offset;
  painter.drawLine( source_x, source_y, dest_x, dest_y );

  // Draw Arrow Heads
  // B is halfway point between two nodes
  /*double const Bx = grid_size * ( n_to.x() + n_from.x() ) / 2 + offset;
  double const By = grid_size * ( n_to.y() + n_from.y() ) / 2 + offset;
  double const arrow_length = grid_size * arrow_length_coeff_;
  Line2D line1 = new Line2D.Double( Bx, By, Bx + arrow_length,
    By - arrow_length );
  Line2D line2 = new Line2D.Double( Bx, By, Bx + arrow_length,
    By + arrow_length );

  double const theta = Math.atan2( source_y - By, source_x - Bx );
  AffineTransform at = AffineTransform.getRotateInstance( theta,
    line1.getX1(), line1.getY1() );

  g2D.draw( at.createTransformedShape( line1 ) );
  g2D.draw( at.createTransformedShape( line2 ) );
  box_for_edge_.put( e, new Box( (int) ( Bx - grid_size ),
      (int) ( By - grid_size ), grid_size * 2, grid_size * 2 ) );*/
}


}//namespace view
