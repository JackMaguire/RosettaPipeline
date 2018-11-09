#include <widgets/EditFileDialog.hh>

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

EditFileDialog::EditFileDialog(
  ExtraFileWidget * parent,
  graph::ExtraFileSP extra_file
) :
  WDialog( "TODO" ),
  parent_( parent ),
  extra_file_( std::move( extra_file ) )
{
  Wt::WContainerWidget * const my_contents = contents();
  Wt::WBorderLayout * const layout =
    my_contents->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Editing " + extra_file_->name ), Wt::LayoutPosition::North );

  Wt::WPushButton * const close_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( ), Wt::LayoutPosition::South );

  close_button->clicked()->connect(
    [=]{
      assert( parent_ );
      parent_->removeChild( this );
    }
  );

  Wt::WTextArea * const edit_area =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( extra_file_->contents ), Wt::LayoutPosition::Center);

  edit_area->changed().connect(
    [=] {
      extra_file_->contents = edit_area->text().toUTF8();
    }
  );

}

EditFileDialog::~EditFileDialog(){}


}//namespace widgets
