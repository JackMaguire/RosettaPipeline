#pragma once

#include <Wt/WDialog.h>
#include <graph/Graph.fwd.hh>

namespace widgets {

class ExtraFileWidget;

class RenameFileDialog : public Wt::WDialog
{
public:
  RenameFileDialog(
    ExtraFileWidget * parent,
    graph::ExtraFileSP extra_file
  );

  virtual ~RenameFileDialog();

private:
  ExtraFileWidget * parent_;
  graph::ExtraFileSP extra_file_;
};

}//namespace widgets
