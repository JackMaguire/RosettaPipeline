#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>

//#include <Wt/WDoubleSpinBox.h>
//#include <Wt/Chart/WCartesianChart.h>

#include <Wt/WPaintDevice.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainter.h>
#include <Wt/WSpinBox.h>

#include <string>

// c++0x only, for std::bind
// #include <functional>

using namespace Wt;

class MyPaintedWidget : public Wt::WPaintedWidget
{
public:
  MyPaintedWidget(Wt::WContainerWidget *parent = 0)
    : Wt::WPaintedWidget(parent), end_(100)
  {
    resize(200, 60);   // Provide a default size.
  }

  void setEnd(int end) {
    end_ = end;
    update();          // Trigger a repaint.
  }

protected:
  void paintEvent(Wt::WPaintDevice *paintDevice) {
    Wt::WPainter painter(paintDevice);
    painter.setBrush(Wt::WBrush(Wt::blue));
    painter.drawRect(0, 0 ,end_, 50);

    painter.setBrush(Wt::WBrush(Wt::black));
    painter.drawEllipse( 10, 10, 10, 10 );
    painter.drawEllipse( 20, 20, 5, 5 );
  }

private:
  int end_;
};

/*
 * A simple hello world application class which demonstrates how to react
 * to events, read input, and give feed-back.
 */
class HelloApplication : public WApplication
{
public:
  HelloApplication(const WEnvironment& env);

private:
  WLineEdit *nameEdit_;
  WText *greeting_;
  int num_button_clicks_;

  void greet();
};

/*
 * The env argument contains information about the new session, and
 * the initial request. It must be passed to the WApplication
 * constructor so it is typically also an argument for your custom
 * application constructor.
 */
HelloApplication::HelloApplication(const WEnvironment& env)
  : WApplication(env)
{
  setTitle("Hello world");                               // application title
  num_button_clicks_ = 0;

  root()->addWidget(new WText("Your name, please ? "));  // show some text
  nameEdit_ = new WLineEdit(root());                     // allow text input
  nameEdit_->setFocus();                                 // give focus

  WPushButton *button
    = new WPushButton("Greet me.", root());              // create a button
  button->setMargin(5, Left);                            // add 5 pixels margin

  root()->addWidget(new WBreak());                       // insert a line break

  greeting_ = new WText(root());                         // empty text
  greeting_->setText( "Number of button clicks: " + std::to_string( num_button_clicks_ ) );


  root()->addWidget(new WBreak());
  //root()->addWidget(new Chart::WCartesianChart());

  Wt::WContainerWidget *container = new Wt::WContainerWidget();
  MyPaintedWidget *painting = new MyPaintedWidget(container);
  root()->addWidget( container );

  /*
   * Connect signals with slots
   *
   * - simple Wt-way
   */
  //button->clicked().connect(this, &HelloApplication::greet);

  /*
   * - using an arbitrary function object (binding values with boost::bind())
   */
  /*nameEdit_->enterPressed().connect
    (boost::bind(&HelloApplication::greet, this));*/

  /*
   * - using a c++0x lambda:
   */
   button->clicked().connect(std::bind([=]() { 
	 greeting_->setText( "Number of button clicks: " + std::to_string( ++num_button_clicks_ ) );
   }));
}

void HelloApplication::greet()
{
  /*
   * Update the text, using text input into the nameEdit_ field.
   */
  greeting_->setText("Hello there, " + nameEdit_->text());
}

WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  return new HelloApplication(env);
}

int main(int argc, char **argv)
{
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  return WRun(argc, argv, &createApplication);
}
