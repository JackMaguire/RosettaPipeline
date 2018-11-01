#include <view/WelcomeWidget.hh>

#include <Wt/WLength.h>
#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WLink.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

namespace view {

namespace {

void
potentially_add_temporary_message( Wt::WBorderLayout * layout ){
  std::ifstream in( "/tmp/message" );
  if( in.is_open() ) {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    std::string const message = contents.str();

    Wt::WText * const text =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( message ), Wt::LayoutPosition::North );

    text->setStyleClass("alert alert-block");

    /*Options appear to be:
    alert-danger (red)
    alert-success (green)
    alert-info (blue)
    alert-block
    alert-error
    */
  }
}

//TODO describe
class CustomContainerWidget : public Wt::WContainerWidget {
public:
  CustomContainerWidget *
  addMessage( std::string const & message ){
    addWidget( Wt::cpp14::make_unique< Wt::WText >( message ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    return this;
  }

  CustomContainerWidget *
  addExtraBreak(){
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    return this;
  }
};

}

WelcomeWidget::WelcomeWidget() :
  WContainerWidget()
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  potentially_add_temporary_message( layout );

  CustomContainerWidget * const main_container =
    layout->addWidget( Wt::cpp14::make_unique< CustomContainerWidget >(), Wt::LayoutPosition::Center );
  //CustomLayout * const main_layout =
    //main_container->setLayout( Wt::cpp14::make_unique< CustomLayout >() );

  main_container->addMessage( "Welcome to RosettaPipeline!" )
    ->addExtraBreak()
    ->addMessage( "Test" )
    ;
  
}

WelcomeWidget::~WelcomeWidget(){}

}//namespace view
