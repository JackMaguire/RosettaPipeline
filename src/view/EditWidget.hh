#pragma once

#include <Wt/WBorderLayout.h>
#include <Wt/WContainerWidget.h>
#include <graph/Graph.hh>

namespace view {

class EditWidget : public Wt::WContainerWidget, graph::SelectionChangeListener
{
public:
  EditWidget( graph::GraphSP );

  virtual ~EditWidget();

  void layoutSizeChanged( int w, int h ) override;

  void noteChangeInSelection() override;

private:
  graph::GraphSP graph_;

  Wt::WBorderLayout * layout_;
  Wt::WContainerWidget * current_;

  int width_;
  int height_;
};

inline
void
EditWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}


}//namespace view
