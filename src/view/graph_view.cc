#include <view/graph_view.hh>
#include <global_data/options.hh>

namespace view {

GraphWidget::GraphWidget() :
  WPaintedWidget(),
  graph_( 0 ),
  theme_( 0 ),
  my_width_( 200 ),
  my_height_( 60 )
{
  //setLayoutSizeAware( true );
  resize( my_width_, my_height_ ); // Provide a default size.
}

GraphWidget::~GraphWidget(){}

void
GraphWidget::paintEvent( Wt::WPaintDevice * paintDevice ) {
  Wt::WPainter painter(paintDevice);
  painter.setBrush( Wt::WBrush( theme_->background() ) );
  painter.drawRect( 0, 0, my_width_, my_height_ );

  int const grid_size = global_data::Options::grid_size;
}


}//namespace view
