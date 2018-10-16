#include <Wt/WContainerWidget.h>
#include <Wt/WPaintDevice.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainter.h>
#include <Wt/WSpinBox.h>

namespace devel {

class MyPaintedWidget : public Wt::WPaintedWidget
{
public:
  MyPaintedWidget()
    : WPaintedWidget(), end_(100)
  {
    //setLayoutSizeAware( true );
    resize(200, 60);   // Provide a default size.
  }

  void setEnd(int end) {
    end_ = end;
    update();          // Trigger a repaint.
  }

  /*void layoutSizeChanged( int w, int h ) override {
    }*/
};

protected:
  void paintEvent(Wt::WPaintDevice *paintDevice) {
    Wt::WPainter painter(paintDevice);
    painter.setBrush(Wt::WBrush(Wt::WColor(Wt::StandardColor::Blue)));
    painter.drawRect(0, 0, end_, 50);
  }

private:
  int end_;
};

};
