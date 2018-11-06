#include <view/PublishWidget.hh>

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

PublishWidget::PublishWidget(
  graph::GraphSP graph,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( std::move( options ) )
{
  addWidget( Wt::cpp14::make_unique< Wt::WText >(
      "Save the current workflow to our server. "
      "It will be given a unique key that can be used to load it later. "
      "Marking it as public will allow other users to be able to search for it. "
      "Otherwise, the user will need to submit the key to load it."
    ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * const publish_Button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Publish" ) );

  auto string_generating_func = [=] {
    return ss.str();
  };

  publish_button->clicked().connect(
    [=] {
      std::string const key = util::generate_random_string( 12 );

      std::string const directory_name = "/published_poses/" + key;
      std::filesystem::create_directory( directory_name );

      std::vector< std::string > save_lines;
      options_->save( save_lines );
      graph->saveSelfNodesAndEdges( save_lines );
    
      std::stringstream ss;
      for( std::string const & line : save_lines ){
	ss << line << "\n";
      }

      {//save file
	std::ofstream save_file;
	save_file.open( directory_name + "/save.txt" );
	save_file << ss.str() << "\n";
	save_file.close();
      }

      {//info file
	std::ofstream info_file;
	info_file.open( directory_name + "/info.txt" );
	info_file << "Title " << "TODO" << "\n";
	info_file << "Author " << "TODO" << "\n";
	info_file << "Tags " << "TODO" << "\n";
	info_file.close();
      }
    }
  );

}

PublishWidget::~PublishWidget(){}

}//namespace view
