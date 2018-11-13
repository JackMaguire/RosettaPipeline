#pragma once

#include <Wt/WDialog.h>
#include <graph/Graph.fwd.hh>

namespace widgets {

class ExtraFileWidget;

class EditFileDialog : public Wt::WDialog
{
public:
  EditFileDialog(
    ExtraFileWidget * parent,
    graph::ExtraFileSP extra_file
  );

  virtual ~EditFileDialog();

private:
  ExtraFileWidget * parent_;
  graph::ExtraFileSP extra_file_;
};

}//namespace widgets
