#pragma once

#include <Wt/WTabWidget.h>
#include <Wt/WGroupBox.h>
#include <Wt/WGlobal.h>

#include <graph/Graph.hh>
#include <widgets/GraphWidget.fwd.hh>
#include <widgets/WelcomeWidget.hh>
#include <widgets/ExamplesWidget.hh>
#include <widgets/NodeWidget.hh>
#include <widgets/EdgeWidget.hh>
#include <widgets/OptionsWidget.hh>
#include <widgets/CompileWidget.hh>

#include <string>

#include <Options.hh>

namespace widgets {

//TODO make .fwd.hh fails
class SaveWidget;
class LoadWidget;
class OptionsWidget;

class SaveAndLoadWidget : public Wt::WContainerWidget {
public:
  SaveAndLoadWidget(
    RightViewWidget * parent,
    graph::GraphSP graph,
    GraphWidget * graph_widget,
    OptionsSP
  );

  ~SaveAndLoadWidget();

  void setOptions( OptionsSP options );

private:
  SaveWidget * save_widget_;
  LoadWidget * load_widget_;
};


class RightViewWidget : public Wt::WTabWidget, graph::SelectionChangeListener, public OptionsHolder
{
public:
  RightViewWidget(
    graph::GraphSP,
    GraphWidget * graph_widget,
    OptionsSP
  );

  ~RightViewWidget() override;

  void layoutSizeChanged( int w, int h ) override;

  void noteChangeInSelection() override;

  //Don't just set the options for this class, but for all the nested data too
  void setOptionsDownward( OptionsSP );

public:
  WelcomeWidget * welcome_widget(){ return welcome_widget_; }
  ExamplesWidget * examples_widget(){ return examples_widget_; }
  NodeWidget * node_edit_widget(){ return node_edit_widget_; }
  EdgeWidget * edge_edit_widget(){ return edge_edit_widget_; }
  OptionsWidget * options_widget(){ return options_widget_; }
  SaveAndLoadWidget * save_and_load_widget(){ return save_and_load_widget_; }
  CompileWidget * compile_widget(){ return compile_widget_; }

protected:
  template< typename T, typename... Args >
  T * addTab_tmpl( std::string tab_title, Args&&... args );

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
RightViewWidget::addTab_tmpl( std::string tab_title, Args&&... args ){
  auto uniq_ptr = Wt::cpp14::make_unique< T >( std::forward< Args >( args )... );
  T * ptr = &(*uniq_ptr);
  assert( ptr );
  addTab( std::move(uniq_ptr), tab_title, Wt::ContentLoading::Eager );  
  return ptr;
}

}//namespace widgets