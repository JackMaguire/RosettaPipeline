#include <view/CompileWidget.hh>
#include <compile/compile.hh>

#include <graph/Graph.fwd.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WText.h>
#include <Wt/WLink.h>
#include <Wt/WStreamResource.h>
#include <Wt/WBreak.h>
#include <Wt/WPushButton.h>
#include <Wt/WMessageBox.h>

#include <iostream>
#include <memory>

namespace view {

namespace {

class OnTheFlyFileResource : public Wt::WStreamResource, public OptionsHolder {
public:
  OnTheFlyFileResource(
    graph::GraphSP graph,
    Wt::WContainerWidget * root,
    OptionsSP options
  ) :
    OptionsHolder( std::move( options ) ),
    graph_( std::move( graph ) ),
    root_( root )
  {}

  ~OnTheFlyFileResource() = default;

  void handleRequest(
    Wt::Http::Request const & request,
    Wt::Http::Response & response
  ) override {

    if( ! request.continuation() ){
      compile::CompilationResult const compilation_result = compile::compile( * graph_, * options_ );
      if( compilation_result.success ){
	iss_for_most_recent_request_ = std::make_unique< std::istringstream >( compilation_result.result );
      } else {
	handleFailure( compilation_result.result );
	return;
      }
    }

    handleRequestPiecewise( request, response, * iss_for_most_recent_request_ );
  }

  void
  handleFailure( std::string const & message ){
    Wt::WMessageBox * const messageBox = root_->addChild(
      Wt::cpp14::make_unique< Wt::WMessageBox >(
	"Error",
	"Compilation failed with message: " + message,
	Wt::Icon::Critical, Wt::StandardButton::Ok
      )
    );
    messageBox->setModal( false );
    messageBox->buttonClicked().connect(
      [=] {
	root_->removeChild( messageBox );
      }
    );
    messageBox->show();
  }


private:
  graph::GraphSP graph_;
  Wt::WContainerWidget * root_;
  std::unique_ptr< std::istringstream > iss_for_most_recent_request_;
};


struct CompileElements {
  CompileElements( Wt::WContainerWidget * root ){
    std::string const description1 =
      "The \"Compile\" button will download a compressed tar file containing everything you need to run this pipeline. "
      "This includes a working directory for every stage and a run.sh script. "
      "In theory, you can just run the run.sh script and the workflow will run as designed. "
      "If any stage crashes, you can fix the problem and simple execute run.sh again; "
      "Rosetta will pick up where it left off (assuming you omit the -overwrite flag). ";
    root->addWidget( Wt::cpp14::make_unique< Wt::WText >( description1 ) );
    root->addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
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
  graph::GraphSP graph,
  OptionsSP options
) :
  WContainerWidget( ),
  OptionsHolder( std::move( options ) )
{ 
  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  Wt::WContainerWidget * const top_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WContainerWidget * const bottom_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );

  /////
  //top
  CompileElements compile_elements( top_container );

  //using ResourceSP = std::shared_ptr< OnTheFlyFileResource >;
  auto file = std::make_shared< OnTheFlyFileResource >( graph, this, options_ );
  file->setDispositionType( Wt::ContentDisposition::Attachment );
  file->suggestFileName( "rosetta_pipeline.tar.gz" );
  compile_elements.compile_button->setLink( Wt::WLink( file ) );

  ////////
  //bottom

  PreviewElements preview_elements( bottom_container );

  preview_elements.preview_button->clicked().connect(
    [=](){
      preview_elements.run_script_area->setText( compile::just_compile_run_script( * graph, options_ ) );
    }
  );
}

CompileWidget::~CompileWidget(){}

}//namespace view
