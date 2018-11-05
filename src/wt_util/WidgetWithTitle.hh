#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WGlobal.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WText.h>


namespace wt_util {

template< typename T, typename... Args >
class WidgetWithTitle : public Wt::WContainerWidget
{
public:
  WidgetWithTitle( std::string const & title, Args&&... args ) :
    WContainerWidget()
  {
    Wt::WBorderLayout * const layout =
      setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
    layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( title ),
      Wt::LayoutPosition::West );

    subwidget_ = 
      layout->addWidget( Wt::cpp14::make_unique< T >( std::forward< Args >( args )... ),
	Wt::LayoutPosition::Center );
  }

  virtual ~WidgetWithTitle(){}

  T * subwidget(){ return subwidget_; }

private:
  T * subwidget_;

};

}//namespace view
