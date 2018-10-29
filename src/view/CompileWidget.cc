#include <view/CompileWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>

#include <iostream>
#include <memory>

namespace view {

CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget( )
{
  auto vbox = setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  Wt::WContainerWidget * const setup_container =
    vbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  Wt::WBorderLayout * const setup_layout =
    setup_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );


  Wt::WContainerWidget * const run_container =
    vbox->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  Wt::WBorderLayout * const run_layout =
    run_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

}

CompileWidget::~CompileWidget(){}

}//namespace view
