#pragma once

#include <Wt/WContainerWidget.h>
#include <Wt/WGlobal.h>
#include <Wt/WMessageBox.h>

namespace wt_util {

//inline
template< typename T >
void
handleFailure( T * root, std::string const & message ){
  Wt::WMessageBox * const messageBox = root->addChild(
    Wt::cpp14::make_unique< Wt::WMessageBox >(
      "Error",
      message,
      Wt::Icon::Critical, Wt::StandardButton::Ok
    )
  );
  messageBox->setModal( false );
  messageBox->buttonClicked().connect(
    [=] {
      root->removeChild( messageBox );
    }
  );
  messageBox->show();
}

//inline
template< typename T >
void
handleSuccess( T * root, std::string const & message ){
  Wt::WMessageBox * const messageBox = root->addChild(
    Wt::cpp14::make_unique< Wt::WMessageBox >(
      "Error",
      message,
      Wt::Icon::Critical, Wt::StandardButton::Ok
    )
  );
  messageBox->setModal( false );
  messageBox->buttonClicked().connect(
    [=] {
      root->removeChild( messageBox );
    }
  );
  messageBox->show();
}


}//namespace wt_util
