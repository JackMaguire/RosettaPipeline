#include <view/GraphToolbarWidget.hh>
#include <view/RightViewWidget.hh>
#include <view/LeftViewWidget.hh>
#include <view/GraphWidget.hh>

//#include <global_data/options.hh>

#include <Wt/WLength.h>
#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WLink.h>
#include <Wt/WPushButton.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

GraphToolbarWidget::GraphToolbarWidget() :
  WContainerWidget()
{
  Wt::WPushButton * const delete_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >() );
  delete_button->setIcon( Wt::WLink( "media/trash.png" ) );
  delete_button->setMinimumSize( 15, 15 );
  delete_button->setMaximumSize( 15, 15 );

}

GraphToolbarWidget::~GraphToolbarWidget(){}

}//namespace view
