#include <view/CompileWidget.hh>
#include <compile/compile.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WPushButton.h>

#include <iostream>
#include <memory>

namespace view {

namespace {

struct CompileElements {
  CompileElements( Wt::WContainerWidget * root ){
    std::string const description1 =
      "The \"Compile\" button will download a compressed tar file containing everything you need to run this pipeline. "
      ;
    root->addWidget( Wt::cpp14::make_unique< Wt::WText >( description1 ) );
    root->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    std::string const description2 =
      "By default, the file will download as rosetta_pipeline.tar.gz to your default location. "
      "To pick a different filename and/or destination, right click on the button and choose \"Save Link As...\""
      ;
    root->addWidget( Wt::cpp14::make_unique< Wt::WText >( description2 ) );
    root->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    std::string const description3 =
      "You can hit the \"Preview\" button to see what the final run.sh script will look like "
      "without having to download and un-tar everything."
      ;
    root->addWidget( Wt::cpp14::make_unique< Wt::WText >( description3 ) );
    root->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    compile_button = root->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Compile" ) );
    compile_button->setStyleClass( "btn-primary" );
  }

  Wt::WPushButton * compile_button;//A
};

struct PreviewElements {
  PreviewElements( Wt::WContainerWidget * root ){
    Wt::WBorderLayout * const border_layout =
      root->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //A
    preview_button =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Preview" ), Wt::LayoutPosition::North );

    Wt::WContainerWidget * const center_container =
      border_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

    Wt::WBorderLayout * const center_layout =
      center_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    //B
    center_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Run.sh" ), Wt::LayoutPosition::North );

    //C
    run_script_area =
      center_layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >(), Wt::LayoutPosition::Center );

  }

  Wt::WPushButton * preview_button;//A
  Wt::WTextArea * run_script_area;//C
};

}


CompileWidget::CompileWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget( )
{ 
  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  Wt::WContainerWidget * const top_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WContainerWidget * const bottom_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  /////
  //top
  CompileElements( top_container );


  ////////
  //bottom

  PreviewElements preview_elements( bottom_container );

  preview_elements.preview_button->clicked().connect(
    [=](){
      preview_elements.run_script_area->setText( compile::just_compile_run_script( * graph ) );
    }
  );
}

CompileWidget::~CompileWidget(){}

}//namespace view
