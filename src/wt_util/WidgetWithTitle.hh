#pragma once

#include <Wt/WContainerWidget.h>
//#include <graph/Graph.fwd.hh>

//#include <Wt/WLength.h>
//#include <Wt/WImage.h>
#include <Wt/WGlobal.h>
//#include <Wt/WHBoxLayout.h>
//#include <Wt/WVBoxLayout.h>
//#include <Wt/WBorderLayout.h>
//#include <Wt/WLink.h>


namespace wt_util {


template< typename T, typename... Args >
class WidgetWithTitle : public Wt::WContainerWidget
{
public:
  WidgetWithTitle() :
    WContainerWidget()
  {
    
  }

  virtual ~WidgetWithTitle(){}

  T * subwidget(){ return subwidget_; }

private:
  T * subwidget_;

};

}//namespace view
