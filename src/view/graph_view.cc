#include <view/graph_view.hh>
#include <global_data/options.hh>

namespace view {

GraphWidget::GraphWidget() :
  WPaintedWidget(),
  graph_( 0 ),
  theme_( 0 ),
  width_( 200 ),
  height_( 60 )
{
  //setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.
}


void
GraphWidget::paintEvent( Wt::WPaintDevice * paintDevice ) {
  Wt::WPainter painter(paintDevice);
  painter.setBrush( Wt::WBrush( theme_->background() ) );
  painter.drawRect( 0, 0, width_, height_ );

  int const grid_size = global_data::Options::grid_size;
}


}//namespace view
