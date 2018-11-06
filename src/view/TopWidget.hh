#pragma once

#include <Wt/WContainerWidget.h>
#include <graph/Graph.fwd.hh>
#include <Options.hh>
#include <iostream>

namespace view {

class TopWidget : public Wt::WContainerWidget
{
public:
  TopWidget(
    graph::GraphSP,
    OptionsSP
  );

  virtual ~TopWidget();

  void layoutSizeChanged( int w, int h ) override;

  void parentResized( Wt::WWidget *parent, Wt::WFlags< Wt::Orientation > directions ) override;

private:
  int width_;
  int height_;
};

inline
void
TopWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
  std::cout << "TopWidget(2) " << "Width " << w << " Height " << h << std::endl;
}

inline
void
TopWidget::parentResized( Wt::WWidget * parent, Wt::WFlags< Wt::Orientation > orientation ) {
  WContainerWidget::parentResized( parent, orientation );
  std::cout << "TopWidget " << "Width " << width().value() << " Height " << height().value() << std::endl;
}


}//namespace view
