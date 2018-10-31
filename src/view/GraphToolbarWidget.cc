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
  WContainerWidget(),
  current_interaction_( GraphInteraction::SELECT )
{
  /*Wt::WBorderLayout * const topmost_layout =
    setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  Wt::WContainerWidget * const container =
    topmost_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >(),
      Wt::LayoutPosition::East );*/

  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  Wt::WPushButton * const select_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >() );
  Wt::WPushButton * const add_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >() );
  Wt::WPushButton * const delete_button =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WPushButton >() );

  select_button->setIcon( Wt::WLink( "media/mouse.png" ) );
  select_button->setMaximumSize( 45, 45 );
  add_button->setIcon( Wt::WLink( "media/plus.png" ) );
  add_button->setMaximumSize( 45, 45 );
  delete_button->setIcon( Wt::WLink( "media/trash.png" ) );
  delete_button->setMaximumSize( 45, 45 );

  select_button->setCheckable( true );
  add_button->setCheckable( true );
  delete_button->setCheckable( true );

  select_button->setChecked( true );
  add_button->setChecked( false );
  delete_button->setChecked( false );

  select_button->checked().connect(
    [=] {
      add_button->setChecked( false );
      delete_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::SELECT;
    }
  );

  add_button->checked().connect(
    [=] {
      select_button->setChecked( false );
      delete_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::ADD;
    }
  );

  delete_button->checked().connect(
    [=] {
      add_button->setChecked( false );
      select_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::DELETE;
    }
  );



  select_button->unChecked().connect(
    [=] {
      select_button->setChecked( true );
      add_button->setChecked( false );
      delete_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::SELECT;
    }
  );

  add_button->unChecked().connect(
    [=] {
      select_button->setChecked( true );
      delete_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::SELECT;
    }
  );

  delete_button->unChecked().connect(
    [=] {
      select_button->setChecked( true );
      add_button->setChecked( false );
      this->current_interaction_ = GraphInteraction::SELECT;
    }
  );


}

GraphToolbarWidget::~GraphToolbarWidget(){}

}//namespace view
