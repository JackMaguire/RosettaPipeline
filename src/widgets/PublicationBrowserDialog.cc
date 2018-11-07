#include <widgets/PublicationBrowserDialog.hh>
#include <widgets/RightViewWidget.hh>
#include <widgets/LeftViewWidget.hh>
#include <widgets/GraphWidget.hh>
#include <widgets/GraphToolbarWidget.hh>

#include <Wt/WLength.h>
//#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
//#include <Wt/WHBoxLayout.h>
//#include <Wt/WVBoxLayout.h>
//#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>

namespace widgets {

PublicationBrowserDialog::PublicationBrowserDialog( ) :
  WDialog( "Published Workflows" )
{
  auto my_contents = contents();
  Wt::WTable * const table =
    my_contents->addWidget( Wt::cpp14::make_unique< Wt::WTable >() );
  table->addStyleClass( "table form-inline" );
  table->setWidth( Wt::WLength( "100%" ) );

  table->setHeaderCount( 1 );
  table->elementAt( 0, 0 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Title" ) );
  table->elementAt( 0, 1 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Author" ) );
  table->elementAt( 0, 2 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Tags" ) );
  table->elementAt( 0, 3 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Key" ) );
  table->elementAt( 0, 4 )->addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ) );



}

PublicationBrowserDialog::~PublicationBrowserDialog(){}

}//namespace widgets
