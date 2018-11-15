#include <widgets/PublishWidget.hh>

#include <graph/Graph.hh>
#include <util.hh>
#include <serialization.hh>

#include <Wt/WGlobal.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WCheckBox.h>

#include <wt_util/WidgetWithTitle.hh>
#include <wt_util/Messages.hh>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstdio> //std::tmpfile
#include <stdio.h>//remove
#include <sstream>
#include <filesystem>//remove

namespace widgets {


PublishWidget::PublishWidget(
  graph::GraphSP graph,
  OptionsSP options
) :
  WContainerWidget(),
  OptionsHolder( options )
{
  addWidget( Wt::cpp14::make_unique< Wt::WText >(
      "Save the current workflow to our server. "
      "It will be given a unique key that can be used to load it later. "
      "Marking it as public will allow other users to be able to search for it. "
      "Otherwise, the user will need to submit the key to load it."
    ) );
  addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

  Wt::WContainerWidget * const line_edit_container =
    addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WHBoxLayout * const line_edit_layout =
    line_edit_container->setLayout( Wt::cpp14::make_unique< Wt::WHBoxLayout >() );
  line_edit_container->setMaximumSize( 1000, 60 );

  wt_util::WidgetWithTitle< Wt::WLineEdit > * const title_container =
    line_edit_layout->addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Title:" ) );
  Wt::WLineEdit * const title_edit = title_container->subwidget();
  title_edit->setText( "" );
  title_edit->setMinimumSize( 100, 40 );
  title_edit->setMaximumSize( 1000, 50 );

  wt_util::WidgetWithTitle< Wt::WLineEdit > * const author_container =
    line_edit_layout->addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Author:" ) );
  Wt::WLineEdit * const author_edit = author_container->subwidget();
  author_edit->setText( "" );
  author_edit->setMinimumSize( 100, 40 );
  author_edit->setMaximumSize( 1000, 50 );

  wt_util::WidgetWithTitle< Wt::WLineEdit > * const tags_container =
    line_edit_layout->addWidget( Wt::cpp14::make_unique< wt_util::WidgetWithTitle< Wt::WLineEdit > >( "Tags:" ) );
  Wt::WLineEdit * const tags_edit = tags_container->subwidget();
  tags_edit->setText( "" );
  tags_edit->setMinimumSize( 100, 40 );
  tags_edit->setMaximumSize( 1000, 50 );

  //addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
  Wt::WPushButton * const publish_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Publish" ) );
  addWidget( Wt::cpp14::make_unique< Wt::WText >( "    " ) );
  Wt::WCheckBox * const private_check_box =
    addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Private" ) );

  Wt::WText * const text =
    addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );


  publish_button->clicked().connect(
    [=] {
      if( title_edit->text().toUTF8().size() == 0 ){
	wt_util::handleFailure( this, "Please set a title" );
	return;
      }

      if( author_edit->text().toUTF8().size() == 0 ){
	wt_util::handleFailure( this, "Please set an author" );
	return;
      }

      if( tags_edit->text().toUTF8().size() == 0 ){
	wt_util::handleFailure( this, "Please provide at least one tag" );
	return;
      }

      if( ! std::filesystem::exists( "/published_pipelines" ) ){
	wt_util::handleFailure( this, "This server is not currently set up to save published files, sorry." );
	return;
      }

      std::string const key = util::generate_random_string( 12 );

      std::string const directory_name = "/published_pipelines/" + key;
      std::filesystem::create_directory( directory_name );

      std::string save_file_contents = serialization::save( * options_, * graph );

      {//save file
	std::ofstream save_file;
	save_file.open( directory_name + "/save.txt" );
	save_file << save_file_contents;
	save_file.close();
      }

      {//info file
	std::ofstream info_file;
	info_file.open( directory_name + "/info.txt" );
	info_file << "Title " << title_edit->text().toUTF8() << "\n";
	info_file << "Author " << author_edit->text().toUTF8() << "\n";
	info_file << "Tags " << tags_edit->text().toUTF8() << "\n";
	info_file << "Private? " << ( private_check_box->isChecked() ? "1" : "0" ) << "\n";
	info_file.close();
      }

      wt_util::handleSuccess( this, "Publishing Was Successful! Your key is: " + key + 
	( private_check_box->isChecked() ? ". This was published as private so you will need this key to access what you saved." :"")
      );
      text->setText( "Key: " + key );
    }
  );

}

PublishWidget::~PublishWidget(){}

}//namespace widgets
