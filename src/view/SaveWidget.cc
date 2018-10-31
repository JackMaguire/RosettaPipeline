#include <view/SaveWidget.hh>

#include <global_data/options.hh>
#include <graph/Graph.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WLink.h>
#include <Wt/WAnchor.h>
#include <Wt/WFileResource.h>
#include <Wt/WStreamResource.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstdio> //std::tmpfile
#include <stdio.h>//remove
#include <sstream>

namespace view {

namespace {

class OnTheFlyFileResource : public Wt::WStreamResource {

  OnTheFlyFileResource( std::string contents ) :
    contents_( contents )
  {}

  ~OnTheFlyFileResource() = default;

  void handleRequest( Http::Request const & request, Http::Response & response ) override {
    std::istringstream iss( contents_ );
    handleRequestPiecewise( request, response, iss );
  }

private:
  std::string contents_;
};

}

SaveWidget::SaveWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget(),
  save_filename_ ( std::tmpnam(nullptr) )
{
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "Save functionality works but is still ugly." ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "EVERY time you want to save, you need to hit the \"Save\"" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "button to update the link at the bottom." ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "To save, right-click the link below and hit 'save as'" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  
  //Wt::WLineEdit * line_edit = addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( "MyProtocol.rpf" ) );
  //addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WPushButton * downloadButton = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Save" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WAnchor * downloadLink = addWidget( Wt::cpp14::make_unique< Wt::WAnchor >() );
  downloadLink->setText( "Download" );

  std::cout << "save_filename_: " << save_filename_ << std::endl;

  downloadButton->clicked().connect(
    [=] {
      std::vector< std::string > save_lines;
      global_data::Options::save( save_lines );
      graph->saveSelfNodesAndEdges( save_lines );

      std::ofstream myfile;
      myfile.open( save_filename_ );
      for( std::string const & line : save_lines ){
	myfile << line << "\n";
      }
      myfile.close();

      //Wt::WFileResource
      auto local_file = std::make_shared< Wt::WFileResource >( save_filename_ );
      //local_file->suggestFileName( line_edit->text() );
      local_file->setDispositionType( Wt::ContentDisposition::Attachment );
      downloadLink->setLink( Wt::WLink( local_file ) );
    }
  );
}

SaveWidget::~SaveWidget(){
  remove( save_filename_.c_str() );
}

}//namespace view
