#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WLineEdit.h>
//#include <graph/Graph.fwd.hh>

#include <string>

namespace view {

class ExtraFileWidget : public Wt::WContainerWidget
{
public:
  ExtraFileWidget();

  virtual ~ExtraFileWidget();

  std::string getFileName() const;

  std::string getFileContents() const;

private:
  Wt::WLineEdit * file_name_;
  Wt::WTextArea * content_editor_;
};

inline
std::string
ExtraFileWidget::getFileName() const {
  if( file_name_ ){
    return file_name_->text().narrow();
  } else {
    return "";
  }
}

inline
std::string
ExtraFileWidget::getFileContents() const {
  if( content_editor_ ){
    return content_editor_->text().narrow();
  } else {
    return "";
  }
}

}//namespace view
