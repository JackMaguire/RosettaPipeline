#include <view/WelcomeWidget.hh>

#include <Wt/WLength.h>
#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WLink.h>

#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>

namespace view {

namespace {

void
potentially_add_temporary_message( Wt::WBorderLayout * layout ){
  ifstream in( "/tmp/message" );
  if( in.is_open() ) {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    std::string const message = contents.str();
    layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( message ), Wt::LayoutPosition::North );
  }
}

std::string
welcome_message(){
  return
    "Welcome to Rosetta Pipeline!\n\n\nTest"
    ;
}

}

WelcomeWidget::WelcomeWidget() :
  WContainerWidget()
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  potentially_add_temporary_message( layout );

  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( message ), Wt::LayoutPosition::Center );
}

WelcomeWidget::~WelcomeWidget(){}

}//namespace view
