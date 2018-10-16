#include <Wt/WContainerWidget>
#include <Wt/WHBoxLayout>
#include <Wt/WText>

#include <devel/DummyPanel.hh>

#include <memory>

using namespace Wt;

int main(){
  
  auto container = std::make_shared< Wt::WContainerWidget >();

  //std::unique_ptr< Wt::WLayout > hbox = std::make_unique< Wt::WHBoxLayout >();
  Wt::WHBoxLayout *hbox = new Wt::WHBoxLayout();//demo uses raw pointers
  container->setLayout( hbox );

  //std::unique_ptr< devel::MyPaintedWidget > left_side = std::make_unique< devel::MyPaintedWidget >();
  auto left_side = new devel::MyPaintedWidget();//demos use raw pointers
  hbox->addWidget( left_side );

  // std::unique_ptr< devel::MyPaintedWidget > right_side = std::make_unique< devel::MyPaintedWidget >();
  auto right_side = new devel::MyPaintedWidget();
  hbox->addWidget( right_side );

  return 0;
}
