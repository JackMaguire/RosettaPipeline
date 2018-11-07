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

#include <cassert>
namespace widgets {

namespace {

class SearchBarWidget : public Wt::WContainerWidget{
public:  
  SearchBarWidget( PublicationBrowserDialog * parent ):
    parent_( parent )
  {
    assert( parent_ );
    all_publications_ = publishing::load( "/published_pipelines" );

    Wt::WBorderLayout * const layout =
      setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

    Wt::WComboBox * const combo_box =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WComboBox >(), Wt::LayoutPosition::West );
    combo_box->addItem( "Title" );
    combo_box->addItem( "Author" );
    combo_box->addItem( "Tag" );
    combo_box->addItem( "Key" );
    combo_box->setMaximumSize( 100, 99999 );

    Wt::WLineEdit * const search_field =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >(), Wt::LayoutPosition::Center );
    search_field->setPlaceholderText( "Search" );

    Wt::WPushButton * const search_button =
      layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Search" ), Wt::LayoutPosition::East );

    search_button->clicked().connect(
      [=]{
	std::list< publishing::Publication > elements;
	for( publishing::Publication const & pub : all_publications_ ){
	  switch( combo_box->currentIndex() ){
	    case 0://title
	      if( pub.is_private ) break;
	      if( pub.title == search_field->text().toUTF8() ) elements.push_back( pub );
	      break;
	    case 1://author
	      if( pub.is_private ) break;
	      if( pub.author == search_field->text().toUTF8() ) elements.push_back( pub );
	      break;
	    case 2://tag
	      if( pub.is_private ) break;
	      if( pub.tags == search_field->text().toUTF8() ) elements.push_back( pub );
	      break;
	    case 3://key
	      if( pub.key == search_field->text().toUTF8() ) elements.push_back( pub );
	      break;
	  }
	}
	parent_->reset_table( elements );
      }
    );
  }

  ~SearchBarWidget() = default;

private:
  PublicationBrowserDialog * parent_;
  std::vector< publishing::Publication > all_publications_;
};

}

PublicationBrowserDialog::PublicationBrowserDialog( ) :
  WDialog( "Published Workflows" )
{
  Wt::WContainerWidget * const my_contents = contents();
  Wt::WBorderLayout * const layout =
    my_contents->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WTable * const table_ =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTable >(), Wt::LayoutPosition::Center );

  table_->addStyleClass( "table form-inline" );
  table_->setWidth( Wt::WLength( "100%" ) );

  table_->setHeaderCount( 1 );
  table_->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title" ) );
  table_->elementAt( 0, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Author" ) );
  table_->elementAt( 0, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Tags" ) );
  table_->elementAt( 0, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Key" ) );
  table_->elementAt( 0, 4 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );

  
  SearchBarWidget * const search_bar =
    layout->addWidget( Wt::cpp14::make_unique< SearchBarWidget >( this ), Wt::LayoutPosition::North );
}

PublicationBrowserDialog::~PublicationBrowserDialog(){}

void PublicationBrowserDialog::reset_table (
  std::list< publishing::Publication > const & new_elements
){
  assert( table_ );
  table_->clear();
  table_->setHeaderCount( 1 );
  table_->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title" ) );
  table_->elementAt( 0, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Author" ) );
  table_->elementAt( 0, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Tags" ) );
  //table_->elementAt( 0, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Key" ) );
  table_->elementAt( 0, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );

  int count = 1;
  for( publishing::Publication const & pub : new_elements ){
    table_->elementAt( count, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( pub.title ) );
    table_->elementAt( count, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( pub.author ) );
    table_->elementAt( count, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( pub.tags ) );

    Wt::WPushButton * const load_button =
      table_->elementAt( count, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Load" ) );    

    ++count;
  }
}

}//namespace widgets
