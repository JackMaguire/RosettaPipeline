#include <widgets/ExtraFileWidget.hh>

#include <widgets/EditFileDialog.hh>
#include <widgets/RenameFileDialog.hh>

#include <Wt/WGlobal.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WFileUpload.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WPushButton.h>
#include <Wt/WMessageBox.h>

#include <graph/Graph.hh>

#include <fstream>
#include <cassert>
#include <algorithm>

namespace widgets {

struct UpperExtraFileWidget : public Wt::WContainerWidget {
  UpperExtraFileWidget(){
    addWidget( Wt::cpp14::make_unique< Wt::WText >(
	    "This feature defines a way for you to provide extra files required for your protocol. "
	    "This could be, for example, a .comp file for your aa_composition score term "
	    "or a .wts file if you want to use a custom score function. "
	    "All of these files will exist one directory up from your running directory "
	    "so you should reference myscore.wts as ../myscore.wts. "
    ) );
    addWidget( Wt::cpp14::make_unique< Wt::WText >(
	    "You can upload a file from your computer or start with a blank file. "
	    "You can edit both the file name and contents (assuming it is a text file) in the browser. "
    ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    addWidget( Wt::cpp14::make_unique< Wt::WText >( "Name:" ) );
    line_edit = addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >() );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    file_upload = addWidget( Wt::cpp14::make_unique< Wt::WFileUpload >() );
    file_upload->setFileTextSize( 1000 );//Set the maximum file size to 1000 kB.
    upload_message_area = addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    upload_button = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Upload" ) );
    addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );
    blank_file_button = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Create Blank Text File" ) );

    setup_connections();
  }

  void setup_connections(){
    upload_button->clicked().connect(
      [=] {
	file_upload->upload();
	upload_message_area->setText( "File upload has begun." );
      }
    );

    file_upload->fileTooLarge().connect(
      [=] {
	upload_message_area->setText( "File is too large. Current limit is 1 MB." );
      }
    );
  }

  Wt::WLineEdit * line_edit;
  Wt::WFileUpload * file_upload;
  Wt::WText * upload_message_area;
  Wt::WPushButton * upload_button;
  Wt::WPushButton * blank_file_button;
};

ExtraFileWidget::ExtraFileWidget( graph::GraphSP graph ) :
  WContainerWidget(),
  graph_( std::move( graph ) )
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  UpperExtraFileWidget * const upper_widget =
    layout->addWidget( Wt::cpp14::make_unique< UpperExtraFileWidget >(), Wt::LayoutPosition::North );

  Wt::WContainerWidget * const table_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(), Wt::LayoutPosition::Center );

  table_ = table_container->addWidget( Wt::cpp14::make_unique< Wt::WTable >() );
  table_->addStyleClass( "table form-inline" );
  table_->setWidth( Wt::WLength( "100%" ) );
  update_table();

  upper_widget->file_upload->uploaded().connect(
    [=] {
      auto const filename = upper_widget->file_upload->spoolFileName();
      if( filename.size() > 1 ){

	std::ifstream in( filename, std::ios::in | std::ios::binary );
	assert( in.is_open() );
	std::ostringstream contents;
	contents << in.rdbuf();
	in.close();
	std::string file_contents = contents.str();

	std::string file_name = upper_widget->line_edit->text().toUTF8();
	if( file_name.empty() ){
	  file_name = "file.txt";
	}

	graph_->addExtraFile( file_name, file_contents);

	upper_widget->upload_message_area->setText( "Upload Complete" );
	
	this->update_table();
      }
    }
  );

  upper_widget->blank_file_button->clicked().connect(
    [=] {//TODO ask for filename if empty
      std::string file_name = upper_widget->line_edit->text().toUTF8();
      graph_->addExtraFile( std::move( file_name ), "" );
      this->update_table();      
    }
  );


}

ExtraFileWidget::~ExtraFileWidget(){}

void ExtraFileWidget::update_table(){
  table_->clear();

  if( graph_->extraFiles().empty() ) return;

  table_->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Filename" ) );

  int counter = 1;
  for( graph::ExtraFileSP const & file : graph_->extraFiles() ){
    table_->elementAt( counter, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( file->getName() ) );
    Wt::WPushButton * const rename_button =
      table_->elementAt( counter, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Rename" ) );
    Wt::WPushButton * const edit_button =
      table_->elementAt( counter, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Edit" ) );
    Wt::WPushButton * const delete_button =
      table_->elementAt( counter, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Delete" ) );

    rename_button->clicked().connect(
      [=]{
	RenameFileDialog * const editor = addChild(
	  Wt::cpp14::make_unique< RenameFileDialog >( this, file )
	);
	editor->show();
      }
    );

    edit_button->clicked().connect(
      [=]{
	EditFileDialog * const editor = addChild(
	  Wt::cpp14::make_unique< EditFileDialog >( this, file )
	);
	editor->show();
      }
    );

    delete_button->clicked().connect(
      [=]{
	  Wt::WMessageBox * const messageBox = addChild(
	    Wt::cpp14::make_unique< Wt::WMessageBox >(
	      "Delete File",
	      "<p>Permanently delete file with name " + file->getName() + "?</p>",
	      Wt::Icon::Warning, Wt::StandardButton::Yes | Wt::StandardButton::No
	    )
	  );
	  // messageBox->setModal( false );
	  messageBox->buttonClicked().connect(
	    [=] {
	      if( messageBox->buttonResult() == Wt::StandardButton::Yes ) {
		auto & vec = this->graph_->extraFiles();
		auto iter = std::find( vec.begin(), vec.end(), file );
		assert( iter != vec.end() );
		vec.erase( iter );
		update_table();
	      }
	      this->removeChild( messageBox );
	    }
	  );
	  messageBox->show();
      }
    );

    ++counter;
  }

}

}//namespace widgets
