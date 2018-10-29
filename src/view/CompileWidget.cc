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

/*
                    BorderLayout    VBox     Border x2
|----------------|  --------------
| Compile Button |  NORTH
|----------------|  --------------  ------   ------
|    title       |                           North
|    Setup.sh    |                  Top      Center
|                |
|----------------|   CENTER         ------   ------
|    title       |                           North
|    Run.sh      |                  Bottom   Center
|                |
|----------------|  --------------  ------   ------



Notation

|--------------
|     A              Compile Button
|--------------
|     B              Setup Title
|-------------
|
|     C              Setup Script
|
|--------------
|     D              Run Title
|-------------
|
|     E              Run Script
|
|--------------

*/

namespace view {

namespace {

struct LayoutsAndContainers {
  LayoutsAndContainers( Wt::WContainerWidget * root ){
    border_layout =
      root->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //A
    compile_button =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Compile" ), Wt::LayoutPosition::North );

    center_container =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

    center_layout =
      center_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

    //top_center == B & C
    top_center_container =
      center_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

    top_center_layout =
      top_center_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //top_center == D & E
    bottom_center_container =
      center_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

    bottom_center_layout =
      bottom_center_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //B
    top_center_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Setup.sh" ), Wt::LayoutPosition::North );

    //C
    setup_script_area =
      top_center_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >(), Wt::LayoutPosition::Center );

    //D
    bottom_center_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Run.sh" ), Wt::LayoutPosition::North );

    //E
    run_script_area =
      bottom_center_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >(), Wt::LayoutPosition::Center );
  }

  Wt::WBorderLayout * border_layout;
  Wt::WPushButton * compile_button;//A
  Wt::WContainerWidget * center_container;//BCDE
  Wt::WVBoxLayout * center_layout;//BCDE
  Wt::WContainerWidget * top_center_container;//BC
  Wt::WBorderLayout * top_center_layout;//BC
  Wt::WContainerWidget * bottom_center_container;//DE
  Wt::WBorderLayout * bottom_center_layout;//DE

  Wt::WTextArea * setup_script_area;//C
  Wt::WTextArea * run_script_area;//E
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
      compile::script_pair scripts = compile::compile( * graph );
      elements.setup_script_area->setText( scripts.setup_script );
      elements.run_script_area->setText( scripts.run_script );
    }
  );
}

CompileWidget::~CompileWidget(){}

}//namespace view
