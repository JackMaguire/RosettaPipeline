#pragma once

#include <Wt/WContainerWidget.h>
#include <widgets/GraphWidget.fwd.hh>
#include <widgets/RightViewWidget.hh>

#include <Options.hh>

namespace widgets {

class OptionsWidget : public Wt::WContainerWidget, public OptionsHolder
{
public:
  OptionsWidget(
    RightViewWidget *,
    GraphWidget *,
    OptionsSP
  );

  ~OptionsWidget() override;

  void update();

protected:
  void setup_view_options( GraphWidget * );
  void setup_run_options();

private:
  RightViewWidget * parent_;

  Wt::WPushButton * zoom_out_button_;
  Wt::WPushButton * zoom_in_button_;
  Wt::WCheckBox * show_node_titles_box_;
  Wt::WCheckBox * orient_node_titles_box_;

  Wt::WCheckBox * delete_unused_intermediate_poses_box_;
  Wt::WCheckBox * serialize_intermediate_poses_box_;
  Wt::WSpinBox * num_proc_spin_box_;
  Wt::WLineEdit * command_edit_;
};

}//namespace widgets
