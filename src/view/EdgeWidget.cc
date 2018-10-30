#include <view/EdgeWidget.hh>

#include <global_data/options.hh>
#include <graph/Edge.hh>

#include <Wt/WBorderLayout.h>
#include <Wt/WBreak.h>
#include <Wt/WCheckBox.h>
#include <Wt/WGlobal.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WLineEdit.h>
//#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WTextEdit.h>
#include <Wt/WVBoxLayout.h>


namespace view {

EdgeWidget::EdgeWidget( graph::EdgeSP edge ) :
  WContainerWidget(),
  edge_( std::move( edge ) )
{
  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  
  //Column Name To Sort By
  construct_segment1( layout );
}

EdgeWidget::~EdgeWidget(){
}

void
EdgeWidget::construct_segment1(
  Wt::WVBoxLayout * const outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Column Name To Sort By: " ), Wt::LayoutPosition::West );

  Wt::WLineEdit * const column_name_to_sort_by_line =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WLineEdit >( edge_->columnNameToSortBy() ), Wt::LayoutPosition::Center );
  Wt::WCheckBox * const positive_scores_are_better_box =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Positive Scores Are Better" ), Wt::LayoutPosition::East );


  column_name_to_sort_by_line->textInput().connect(
    [=] {
      edge_->setColumnNameToSortBy( column_name_to_sort_by_line->text().toUTF8() );
    }
  );


  positive_scores_are_better_box->setChecked( edge_->getPositiveScoresAreBetter() );

  positive_scores_are_better_box->checked().connect (
    [=] {
      edge_->setPositiveScoresAreBetter( true );
    }
  );

  positive_scores_are_better_box->unChecked().connect (
    [=] {
      edge_->setPositiveScoresAreBetter( false );
    }
  );


}

}//namespace view
