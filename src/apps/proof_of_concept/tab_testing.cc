#include <Wt/WContainerWidget.h>
#include <Wt/WMenuItem.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WApplication.h>

using namespace Wt;

std::unique_ptr<WApplication> createApplication(const WEnvironment& env)
{
  std::unique_ptr<WApplication> app = Wt::cpp14::make_unique<WApplication>(env);
  auto container = app->root();//Wt::cpp14::make_unique<Wt::WContainerWidget>();

  Wt::WTabWidget *tabW = container->addWidget(Wt::cpp14::make_unique<Wt::WTabWidget>());
  tabW->addTab(Wt::cpp14::make_unique<Wt::WTextArea>("This is the contents of the first tab."),
    "First", Wt::ContentLoading::Eager);
  tabW->addTab(Wt::cpp14::make_unique<Wt::WTextArea>("The contents of the tabs are pre-loaded in"
      " the browser to ensure swift switching."),
    "Preload", Wt::ContentLoading::Eager);
  tabW->addTab(Wt::cpp14::make_unique<Wt::WTextArea>("You could change any other style attribute of the"
      " tab widget by modifying the style class."
      " The style class 'trhead' is applied to this tab."),
    "Style", Wt::ContentLoading::Eager)->setStyleClass("trhead");
  
  Wt::WMenuItem *tab
    = tabW->addTab(Wt::cpp14::make_unique<Wt::WTextArea>("You can close this tab"
	" by clicking on the close icon."),
      "Close");
  tab->setCloseable(true);
  tabW->setStyleClass("tabwidget");

  app->setCssTheme("polished");

  return app;
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}
