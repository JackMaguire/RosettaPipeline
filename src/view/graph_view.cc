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
  painter.setBrush( Wt::WBrush( Wt::WColor( Wt::blue ) ) );
  //painter.drawRect( 0, 0, end_, 50 );
}


}//namespace view
