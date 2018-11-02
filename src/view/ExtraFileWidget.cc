#include <view/ExtraFileWidget.hh>

//#include <global_data/options.hh>

//#include <Wt/WLength.h>
//#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
//#include <Wt/WHBoxLayout.h>
//#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
//#include <Wt/WLink.h>
#include <Wt/WText.h>

namespace view {

ExtraFileWidget::ExtraFileWidget( ) :
  WContainerWidget()
{
  Wt::WBorderLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WContainerWidget * const upper_container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(),
      Wt::LayoutPosition::North );
    
  Wt::WBorderLayout * const upper_layout =
    upper_container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  upper_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Filename:" ),
    Wt::LayoutPosition::West );

  file_name_ =
    upper_layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( "filename" ),
      Wt::LayoutPosition::Center );

  content_editor_ =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( ),
      Wt::LayoutPosition::Center );

}

ExtraFileWidget::~ExtraFileWidget(){}

}//namespace view
