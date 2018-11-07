#pragma once

#include <Wt/WDialog.h>
#include <Wt/WTable.h>
#include <Wt/WContainerWidget.h>

#include <publishing/load.hh>

#include <list>

namespace widgets {

class PublicationBrowserDialog : public Wt::WDialog
{
public:
  PublicationBrowserDialog( Wt::WContainerWidget * parent );

  virtual ~PublicationBrowserDialog();

  void reset_table( std::list< publishing::Publication > const & new_elements );

private:
  Wt::WTable * table_;
};

}//namespace widgets
