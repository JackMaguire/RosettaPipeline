#include <view/SaveWidget.hh>

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

template < typename T >
class OnTheFlyFileResource : public Wt::WStreamResource {
public:
  OnTheFlyFileResource( T string_generating_func ) :
    string_generating_func_( string_generating_func )
  {}

  ~OnTheFlyFileResource() = default;

  void handleRequest(
    Wt::Http::Request const & request,
    Wt::Http::Response & response
  ) override {
    std::istringstream iss( string_generating_func_() );
    handleRequestPiecewise( request, response, iss );
  }

private:
  T string_generating_func_;
};

}

SaveWidget::SaveWidget(
  graph::GraphSP graph,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( std::move( options ) )
{
  Wt::WPushButton * const downloadButton =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Download" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "<b>Tip:</b> Right-click the button and choose \"Save Link As...\" to pick a filename and save location." ) );

  auto string_generating_func = [=] {
    std::vector< std::string > save_lines;
    options_->save( save_lines );
    graph->saveSelfNodesAndEdges( save_lines );
    
    std::stringstream ss;
    for( std::string const & line : save_lines ){
      ss << line << "\n";
    }
    return ss.str();
  };

  using Resource_T = OnTheFlyFileResource< decltype(string_generating_func) >;
  using Resource_TSP = std::shared_ptr< Resource_T >;

  Resource_TSP local_file = std::make_shared< Resource_T >( string_generating_func );
  local_file->setDispositionType( Wt::ContentDisposition::Attachment );
  local_file->suggestFileName( "MyRosettaPipeline.txt" );
  downloadButton->setLink( Wt::WLink( local_file ) );
}

SaveWidget::~SaveWidget(){}

}//namespace view
