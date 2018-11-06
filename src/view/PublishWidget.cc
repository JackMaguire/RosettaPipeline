#include <view/PublishWidget.hh>

#include <graph/Graph.hh>
#include <util.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WLink.h>
#include <Wt/WFileResource.h>
#include <Wt/WStreamResource.h>
#include <Wt/WMessageBox.h>

#include <wt_util/WidgetWithTitle.hh>

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

template< typename T >
void
handleFailure( T * root, std::string const & message ){
  Wt::WMessageBox * const messageBox = root->addChild(
    Wt::cpp14::make_unique< Wt::WMessageBox >(
      "Error",
      message,
      Wt::Icon::Critical, Wt::StandardButton::Ok
    )
  );
  messageBox->setModal( false );
  messageBox->buttonClicked().connect(
    [=] {
      root->removeChild( messageBox );
    }
  );
  messageBox->show();
}


}

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

  Wt::WLineEdit * const title_edit =
    addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Title:" ) )->subwidget();
  title_edit->setText( "" );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WLineEdit * const author_edit =
    addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Author:" ) )->subwidget();
  author_edit->setText( "" );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WLineEdit * const tags_edit =
    addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Tags:" ) )->subwidget();
  tags_edit->setText( "" );

  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * const publish_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Publish" ) );

  publish_button->clicked().connect(
    [=] {
      if( title_edit->text().toUTF8().size() == 0 ){
	handleFailure( this, "Please set a title" );
	return;
      }

      if( author_edit->text().toUTF8().size() == 0 ){
	handleFailure( this, "Please set an author" );
	return;
      }

      if( tags_edit->text().toUTF8().size() == 0 ){
	handleFailure( this, "Please provide at least one tag" );
	return;
      }

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
	info_file << "Title " << title_edit->text().toUTF8() << "\n";
	info_file << "Author " << author_edit->text().toUTF8() << "\n";
	info_file << "Tags " << tags_edit->text().toUTF8() << "\n";
	info_file.close();
      }
    }
  );

}

PublishWidget::~PublishWidget(){}

}//namespace view
