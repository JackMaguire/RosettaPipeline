#include <view/CompileWidget.hh>
#include <compile/compile.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WText.h>
#include <Wt/WPushButton.h>

#include <iostream>
#include <memory>

namespace view {

namespace {

struct LayoutsAndContainers {
  LayoutsAndContainers( Wt::WContainerWidget * root ){
    Wt::WBorderLayout * const border_layout =
      root->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //A
    compile_button =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Compile" ), Wt::LayoutPosition::North );

    Wt::WContainerWidget * const center_container =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

    Wt::WVBoxLayout * const center_layout =
      center_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

    //B
    center_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Run.sh" ), Wt::LayoutPosition::North );

    //C
    run_script_area =
      center_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >(), Wt::LayoutPosition::Center );

  }

  Wt::WPushButton * compile_button;//A
  Wt::WTextArea * run_script_area;//C
};

}


CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget( )
{ 
  LayoutsAndContainers elements( this );

  elements.compile_button->clicked().connect(
    [=](){
      elements.run_script_area->setText( compile::just_compile_run_script( * graph ) );
    }
  );
}

CompileWidget::~CompileWidget(){}

}//namespace view
