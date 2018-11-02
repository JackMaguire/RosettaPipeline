#pragma once

#include <Wt/WTabWidget.h>
#include <Wt/WGlobal.h>

#include <graph/Graph.hh>
#include <view/GraphWidget.fwd.hh>

#include <string>

namespace view {

class WelcomeWidget;
class ExamplesWidget;
class NodeWidget;
class EdgeWidget;
class OptionsWidget;
class SaveAndLoadWidget;
class CompileWidget;

class RightViewWidget : public Wt::WTabWidget, graph::SelectionChangeListener
{
public:
  RightViewWidget(
    graph::GraphSP,
    GraphWidget * graph_widget
  );

  virtual ~RightViewWidget();

  void layoutSizeChanged( int w, int h ) override;

  void noteChangeInSelection() override;

protected:
  template< typename T, typename... Args >
  T * addTab( std::string tab_title, Args&&... args );

private:
  graph::GraphSP graph_;
  GraphWidget * graph_widget_;

  WelcomeWidget * welcome_widget_;
  ExamplesWidget * examples_widget_;
  NodeWidget * node_edit_widget_;
  EdgeWidget * edge_edit_widget_;
  OptionsWidget * options_widget_;
  SaveAndLoadWidget * save_and_load_widget_;
  CompileWidget * compile_widget_;

  int width_;
  int height_;
};

inline
void
RightViewWidget::layoutSizeChanged( int w, int h ) {
  width_ = w;
  height_ = h;
}

//Wt is not friendly about ownership of child widgets
//I would love to just use shared_ptrs, but Wt's only option is to give the container
//a unique_ptr and keep a raw ptr for ourselves
//inline
template< typename T, typename... Args >
T *
RightViewWidget::addTab( std::string tab_title, Args&&... args ){
  auto uniq_ptr = Wt::cpp14::make_unique< T >( std::forward< Args >( args )... );
  addTab( std::move(uniq_ptr), tab_title, Wt::ContentLoading::Eager );  
  return &(*uniq_ptr);
}

}//namespace view
