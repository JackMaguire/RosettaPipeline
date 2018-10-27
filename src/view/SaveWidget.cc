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

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstdio> //std::tmpfile

namespace view {

namespace {

}

SaveWidget::SaveWidget(
  graph::GraphSP const & graph
) :
  WContainerWidget(),
  save_filename_ ( std::tmpnam(nullptr) )
{
  Wt::WLineEdit * line_edit = addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( "MyProtocol.rpf" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WPushButton * downloadButton = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Save" ) );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WAnchor * downloadLink = addWidget( cpp14::make_unique< Wt::WAnchor >() );

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
      local_file->suggestFileName( line_edit->text() );
      local_file->setDispositionType( Wt::Attachment );
      downloadLink->setLink( Wt::WLink( Wt::Resource, local_file ) );
    }
  );
}

SaveWidget::~SaveWidget(){
  std::remove( save_filename_ );
}

}//namespace view
