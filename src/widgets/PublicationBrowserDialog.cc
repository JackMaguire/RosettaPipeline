#include <widgets/PublicationBrowserDialog.hh>
#include <widgets/RightViewWidget.hh>
#include <widgets/LeftViewWidget.hh>
#include <widgets/GraphWidget.hh>
#include <widgets/GraphToolbarWidget.hh>

#include <publishing/load.hh>

#include <Wt/WLength.h>
#include <Wt/WPushButton.h>
#include <Wt/WLineEdit.h>
#include <Wt/WGlobal.h>
#include <Wt/WComboBox.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>

namespace widgets {

namespace {

class SearchBarWidget : public Wt::WContainerWidget(){
public:  
  SearchBarWidget(){

    all_publications_ = publishing::load( "/published_pipelines" );

    Wt::WBorderLayout * const layout =
      setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    Wt::WComboBox * const combo_box =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WComboBox >(), Wt::LayoutPosition::West );
    combo_box->addItem( "Title" );
    combo_box->addItem( "Author" );
    combo_box->addItem( "Tag" );
    combo_box->addItem( "Key" );

    Wt::WLineEdit * const search_field =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >(), Wt::LayoutPosition::Center );
    search_field->setPlaceholderText( "Search" );

    Wt::WPushButton * const search_button =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >(), Wt::LayoutPosition::East );
  }

  ~SearchBarWidget() = default;

private:
  std::vector< publishing::Publication > all_publications_;
};

}

PublicationBrowserDialog::PublicationBrowserDialog( ) :
  WDialog( "Published Workflows" )
{
  Wt::WContainerWidget * const my_contents = contents();
  Wt::WBorderLayout * const layout =
    my_contents->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WTable * const table =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTable >(), Wt::LayoutPosition::Center );

  table->addStyleClass( "table form-inline" );
  table->setWidth( Wt::WLength( "100%" ) );

  table->setHeaderCount( 1 );
  table->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title" ) );
  table->elementAt( 0, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Author" ) );
  table->elementAt( 0, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Tags" ) );
  table->elementAt( 0, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Key" ) );
  table->elementAt( 0, 4 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );

  
  SearchBarWidget * const search_bar =
    layout->addWidget( Wt::cpp14::make_unique< SearchBarWidget >(), Wt::LayoutPosition::North );
}

PublicationBrowserDialog::~PublicationBrowserDialog(){}

}//namespace widgets
