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
    alert-block (yellow)
    alert-error

    info and block seem to be the best ones
    */
  }
}

//TODO describe
class CustomContainerWidget : public Wt::WContainerWidget {
public:
  CustomContainerWidget *
  addMessage( std::string const & message, int num_breaks=2 ){
    addWidget( Wt::cpp14::make_unique< Wt::WText >( message ) );
    for( int i=0; i<num_breaks; ++i )
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

  main_container->addMessage(
    "Welcome to RosettaPipeline!"
  )->addMessage(//What are we?
    "<b>What is RosettaPipeline?</b>", 1
  )->addMessage(//What are we?
    "This is a service that allows Rosetta users to create and share their multistage design workflows. "
    "Workflows are represnted using the graph on the left. "
    "Each node in the graph represents a stage of your pipeline and "
    "each directed edge represents the transfer of poses from one stage to the next. "
    "Similar to multistage_rosetta_scripts, you can filter results between stages to save time. "
    "When you are finished, go to the \"Compile\" tab for instructions on how to download this workflow for use. "
    "If you want to save a workflow, go to the \"Save/Load\" tab to download a text file that can be loaded in to this site later."
  )->addMessage(//How to?
    "<b>How to make a workflow:</b>", 1
  )->addMessage(//How to?
    "To create a workflow, start by editing the graph one the left using the Select, Add, and Delete buttons. "
    "You can create nodes by pushing the plus button and clicking anywhere in the workspace. "
    "Edges are created by clicking and dragging from one node to another. "
    "Select nodes and edges by clicking on them (must click on the center arrow of edges) "
    "and edit their parameters in the \"Edit\" tab. "
    "When your graph is complete, visit the \"Options\" tab to set parameters for your run. "
  )->addMessage(//Getting Started
    "<b>Getting started:</b>", 1
  )->addMessage(//Getting Started
    "If you are new, you can start by exploring sample pipelines in the \"Examples\" tab. "
  );
  
}

WelcomeWidget::~WelcomeWidget(){}

}//namespace view
