#include <Wt2/WContainerWidget.h>
#include <Wt2/WPaintDevice.h>
#include <Wt2/WPaintedWidget.h>
#include <Wt2/WPainter.h>
#include <Wt2/WSpinBox.h>
#include <Wt2/WGlobal.h>

namespace devel {

class MyPaintedWidget : public Wt::WPaintedWidget
{
public:
  MyPaintedWidget() :
    WPaintedWidget(),
    end_( 100 )
  {
    //setLayoutSizeAware( true );
    resize(200, 60);   // Provide a default size.
  }

  /*void layoutSizeChanged( int w, int h ) override {
    }*/

protected:
  void paintEvent(Wt::WPaintDevice *paintDevice) {
    Wt::WPainter painter(paintDevice);
    painter.setBrush( Wt::WBrush( Wt::WColor( Wt::blue ) ) );
    painter.drawRect( 0, 0, end_, 50 );
  }

private:
  int end_;
};

};
