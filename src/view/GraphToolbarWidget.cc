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
  /*Wt::WPushButton * const delete_button =
    addWidget( Wt::cpp14::make_unique< Wt::WPushButton >() );
  delete_button->setIcon( Wt::WLink( "media/trash.png" ) );
  delete_button->setMinimumSize( 25, 25 );
  delete_button->setMaximumSize( 25, 25 );*/

  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  Wt::WPushButton * const select_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Select" ) );
  Wt::WPushButton * const add_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Add" ) );
  Wt::WPushButton * const delete_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >( "Delete" ) );

  select_button->setCheckable( true );
  add_button->setCheckable( true );
  delete_button->setCheckable( true );

  select_button->setChecked( true );
  add_button->setChecked( false );
  delete_button->setChecked( false );

  select_button->checked().connect(
    [add_button,delete_button,&current_interaction_] {
      add_button->setChecked( false );
      delete_button->setChecked( false );
      current_interaction_ = GraphInteraction::SELECT;
    }
  );

  add_button->checked().connect(
    [select_button,delete_button,&current_interaction_] {
      select_button->setChecked( false );
      delete_button->setChecked( false );
      current_interaction_ = GraphInteraction::ADD;
    }
  );

  delete_button->checked().connect(
    [add_button,select_button,&current_interaction_] {
      add_button->setChecked( false );
      select_button->setChecked( false );
      current_interaction_ = GraphInteraction::DELETE;
    }
  );

}

GraphToolbarWidget::~GraphToolbarWidget(){}

}//namespace view
