#include <view/LoadWidget.hh>

#include <global_data/options.hh>
#include <graph/Graph.fwd.hh>

//#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

#include <iostream>
#include <memory>
#include <string>

namespace view {

LoadWidget::LoadWidget(
  graph::GraphSP const & graph
) :
  WFileUpload(),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  //using namespace Wt;

  Wt::WPushButton * uploadButton = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load" ) );
  Wt::WText * out = container->addWidget( Wt::cpp14::make_unique< Wt::WText >() );

  uploadButton->clicked().connect(
    [=] {
      upload();
      uploadButton->disable();
      out->setText("File upload is finished.");
    }
  );

  uploaded().connect(
    [=] {
      uploadButton->enable();
      out->setText( "File upload is finished. Total num files uploaded: " +
	std::to_string( uploadedFiles().size() ) );
    }
  );

  fileTooLarge().connect(
    [=] {
      uploadButton->enable();
      out->setText("File is too large.");
    }
  );
}

LoadWidget::~LoadWidget(){}

}//namespace view
