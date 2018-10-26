#include <view/LoadWidget.hh>

#include <global_data/options.hh>
#include <graph/Graph.fwd.hh>

//#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WFileUpload.h>

#include <iostream>
#include <memory>
#include <string>

namespace view {

LoadWidget::LoadWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget(),
  width_( 500 ),
  height_( 800 )
{
  setLayoutSizeAware( true );
  resize( width_, height_ ); // Provide a default size.

  //largely copied from https://github.com/emweb/wt/blob/29ae91638e197013f67e7c826317529615d10749/examples/widgetgallery/examples/FileUpload.cpp

  Wt::WFileUpload * fu = addWidget( Wt::cpp14::make_unique< Wt::WFileUpload >() );
  fu->setFileTextSize( 50 );//Set the maximum file size to 50 kB.
  fu->setProgressBar( Wt::cpp14::make_unique< Wt::WProgressBar >() );

  Wt::WPushButton * uploadButton = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load" ) );
  Wt::WText * out = addWidget( Wt::cpp14::make_unique< Wt::WText >() );

  uploadButton->clicked().connect(
    [=] {
      fu->upload();
      uploadButton->disable();
      out->setText("File upload is finished.");
    }
  );

  fu->uploaded().connect(
    [=] {
      uploadButton->enable();
      out->setText( "File upload is finished. Total num files uploaded: " +
	std::to_string( uploadedFiles().size() ) );
    }
  );

  fu->fileTooLarge().connect(
    [=] {
      uploadButton->enable();
      out->setText("File is too large.");
    }
  );
}

LoadWidget::~LoadWidget(){}

}//namespace view
