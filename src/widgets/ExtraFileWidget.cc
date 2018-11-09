#include <widgets/ExtraFileWidget.hh>

#include <widgets/EditFileDialog.hh>

#include <Wt/WGlobal.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>
#include <Wt/WLineEdit.h>
#include <Wt/WFileUpload.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WPushButton.h>

#include <graph/Graph.hh>

#include <fstream>

namespace widgets {

struct UpperExtraFileWidget : public Wt::WContainerWidget {
  UpperExtraFileWidget(){
    addWidget( Wt::cpp14::make_unique< Wt::WText >( "TODO: Describe How This Works!" ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    addWidget( Wt::cpp14::make_unique< Wt::WText >( "Name:" ) );
    line_edit = addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >() );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    file_upload = addWidget( Wt::cpp14::make_unique< Wt::WFileUpload >() );
    file_upload->setFileTextSize( 1000 );//Set the maximum file size to 1000 kB.
    upload_message_area = addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );
    addWidget( Wt::cpp14::make_unique< Wt::WBreak >() );

    upload_button = addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Upload" ) );

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
};

ExtraFileWidget::ExtraFileWidget( graph::GraphSP graph ) :
  WContainerWidget(),
  graph_( std::move( graph ) )
{
  global_data::register_refreshable_object( this );

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
	graph_->addExtraFile( file_name, file_contents);

	upper_widget->upload_message_area->setText( "Upload Complete" );
	
	this->update_table();
      }
    }
  );

}

ExtraFileWidget::~ExtraFileWidget(){
  global_data::deregister_refreshable_object( this );
}

void ExtraFileWidget::update_table(){
  table_->clear();
  table_->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Filename" ) );

  int counter = 1;
  for( graph::ExtraFileSP const & file : graph_->extraFiles() ){
    table_->elementAt( counter, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( file->name ) );
    Wt::WPushButton * const edit_button =
      table_->elementAt( counter, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Edit As Text" ) );
    Wt::WPushButton * const delete_button =
      table_->elementAt( counter, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Delete" ) );

    edit_button->clicked().connect(
      [=]{
	EditFileDialog * const editor = addChild(
	  Wt::cpp14::make_unique< EditFileDialog >( this, file )
	);
	editor->show();
      }
    );


    counter++;
  }

}

}//namespace widgets
