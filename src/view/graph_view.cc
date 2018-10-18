#include <view/graph_view.hh>
#include <global_data/options.hh>

#include <Wt/WLength>

namespace view {

GraphWidget::GraphWidget() :
  WPaintedWidget(),
  graph_( 0 ),
  theme_(),
  width_( 200 ),
  height_( 20 )
{
  width_ = width();
  height_ = height();

  setLayoutSizeAware( true );

  resize( WLength::auto, WLength::auto ); // Provide a default size.
}

GraphWidget::~GraphWidget(){}

void
GraphWidget::layoutSizeChanged( int w, int h ) override {
  width_ = w;
  height_ = h;
}

void
GraphWidget::paintEvent( Wt::WPaintDevice * paintDevice ) {
  Wt::WPainter painter(paintDevice);

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
}


}//namespace view
