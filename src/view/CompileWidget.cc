#include <view/CompileWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>

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



*/

namespace view {


namespace {

struct LayoutsAndContainers {
  LayoutsAndContainers( Wt::WContainerWidget * root ){
    border_layout =
      root->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    center_container =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

    center_layout =
      center_container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

    top_center_container =
      center_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  }

  Wt::WBorderLayout * border_layout;
  Wt::WContainerWidget * center_container;
  Wt::WVBoxLayout * center_layout;
  Wt::WContainerWidget * top_center_container;
};

}


CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget( )
{
  Wt::WPushButton * compile_button = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load" ) );
 
  LayoutsAndContainers elements( this );
}

CompileWidget::~CompileWidget(){}

}//namespace view
