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

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

GraphToolbarWidget::GraphToolbarWidget() :
  WContainerWidget()
{
}

GraphToolbarWidget::~GraphToolbarWidget(){}

}//namespace view
