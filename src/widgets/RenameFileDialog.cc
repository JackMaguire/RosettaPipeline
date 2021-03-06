#include <widgets/RenameFileDialog.hh>

#include <widgets/ExtraFileWidget.hh>
#include <graph/Graph.hh>

#include <Wt/WLength.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WGlobal.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>
#include <Wt/WTextArea.h>

#include <cassert>

namespace widgets {

namespace {

}

RenameFileDialog::RenameFileDialog(
  ExtraFileWidget * parent,
  graph::ExtraFileSP extra_file
) :
  WDialog( "Renaming " + extra_file->getName() ),
  parent_( parent ),
  extra_file_( extra_file )
{
  Wt::WContainerWidget * const my_contents = contents();
  my_contents->setMinimumSize( 300, 400 );

  Wt::WBorderLayout * const layout =
    my_contents->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  //layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Editing " + extra_file_->name ), Wt::LayoutPosition::North );

  Wt::WPushButton * const close_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Done" ), Wt::LayoutPosition::South );

  close_button->setMinimumSize( 10, 40 );

  close_button->clicked().connect(
    [=]{
      assert( parent_ );
      parent_->removeChild( this );
    }
  );

  Wt::WLineEdit * const edit_area =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >(), Wt::LayoutPosition::Center );
  edit_area->setText( extra_file_->getName() );

  edit_area->changed().connect(
    [=] {
      extra_file_->setName( edit_area->text().toUTF8() );
      parent->update_table();
    }
  );

}

RenameFileDialog::~RenameFileDialog(){}


}//namespace widgets
