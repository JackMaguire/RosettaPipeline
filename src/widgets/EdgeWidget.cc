#include <widgets/EdgeWidget.hh>

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
#include <Wt/WComboBox.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WSpinBox.h>

#include <limits>

namespace widgets {

EdgeWidget::EdgeWidget( graph::EdgeSP edge ) :
  WContainerWidget(),
  edge_( edge )
{
  Wt::WVBoxLayout * const layout =
    setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );

  add_intro( layout );
  construct_segment1( layout );
  construct_segment2( layout );
  construct_segment3( layout );
  construct_segment4( layout );
  construct_dummy_segment( layout );

  /*
  Wt::WContainerWidget * const segments1and2container =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WVBoxLayout * const segments1and2layout =
    segments1and2container->setLayout( Wt::cpp14::make_unique< Wt::WVBoxLayout >() );
  construct_segment1( segments1and2layout );
  construct_segment2( segments1and2layout );
  */
}

EdgeWidget::~EdgeWidget(){
}

void
EdgeWidget::add_intro(
  Wt::WVBoxLayout * const outer_layout
) {
  std::string const description =
    "An edge represents the process of migrating one node's results into the input pool for the downstream node. "
    "You likely only want to transfer the best results, and the options on this page let you determine which results to keep. "
    "First, you'll want to decided which column in the score.sc file you want to use to measure quality. "
    "We do not offer a way to combine multiple metrics, "
    "but you can use tools such as CalculatorFilter and FilterReportAsPoseExtraScoresMover "
    "within your script to create a custom column in the score.sc file.\n"
    "You then need to decide how many results from the upstream node you want to keep. "
    "You can either declare a maximum number (any integer larger than 0) or a fraction of the results (between 0.0 and 1.0)."
    ;

  outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( description )  );
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
  column_name_to_sort_by_line->setInline( true );
  column_name_to_sort_by_line->setFormObject( true );

  Wt::WCheckBox * const positive_scores_are_better_box =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WCheckBox >( "Positive Scores Are Better" ), Wt::LayoutPosition::East );


  column_name_to_sort_by_line->setMaximumSize( column_name_to_sort_by_line->maximumWidth(), 30 );

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


void
EdgeWidget::construct_segment2(
  Wt::WVBoxLayout * const outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );

  Wt::WComboBox * const combo_box =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WComboBox >(), Wt::LayoutPosition::West );
  Wt::WDoubleSpinBox * const spin_box =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WDoubleSpinBox >(), Wt::LayoutPosition::East );

  combo_box->setMaximumSize( combo_box->maximumWidth(), 30 );
  spin_box->setMaximumSize( spin_box->maximumWidth(), 30 );

  //TODO WStackedWidget

  combo_box->addItem( "Maximum Number of Results to Transfer:" );
  combo_box->addItem( "Fraction of Results to Transfer:" );
  spin_box->setMinimum( 0 );

  if( edge_->useFractionInsteadOfCount() ){
    combo_box->setCurrentIndex( 1 );
    spin_box->setValue( edge_->fractionOfResultsToTransfer() );
    spin_box->setMaximum( 1.0 );
    spin_box->setSingleStep( 0.01 );
  } else {
    combo_box->setCurrentIndex( 0 );
    spin_box->setValue( edge_->numResultsToTransfer() );
    spin_box->setMaximum( std::numeric_limits< int >::max() );
    spin_box->setSingleStep( 1 );
  }

  combo_box->changed().connect(
    [=] {
      bool const use_frac = combo_box->currentIndex() == 1;
      edge_->setUseFractionInsteadOfCount( use_frac );

      if( use_frac ){
	spin_box->setValue( edge_->fractionOfResultsToTransfer() );
	spin_box->setMaximum( 1.0 );
	spin_box->setSingleStep( 0.01 );
      } else {
	spin_box->setValue( edge_->numResultsToTransfer() );
	spin_box->setMaximum( std::numeric_limits< int >::max() );
	spin_box->setSingleStep( 1 );
      }

    }
  );

  spin_box->changed().connect(
    [=] {
      try {
	bool const use_frac = combo_box->currentIndex() == 1;
	if( use_frac ){
	  edge_->setFractionOfResultsToTransfer( spin_box->value() );
	} else {
	  edge_->setNumResultsToTransfer( int( spin_box->value() ) );
	}
      } catch (...){}
    }
  );
}

void
EdgeWidget::construct_segment3(
  Wt::WVBoxLayout * const outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Notes:" ),
    Wt::LayoutPosition::North );
  Wt::WTextArea * const text_area =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( edge_->notes() ),
      Wt::LayoutPosition::Center );
  text_area->setMinimumSize( 400, 100 );

  text_area->changed().connect(
    [=] {
      edge_->setNotes( text_area->text().toUTF8() );
    }
  );
}

void
EdgeWidget::construct_segment4(
  Wt::WVBoxLayout * const outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "Help:" ),
    Wt::LayoutPosition::North );

  std::string const help_message =
    "Click on a node to select a node\n"
    "Click on the arrow of an edge to select that edge\n"
    "Shift+Click on a node/edge to delete it\n"
    "Alt+Click to create a node\n"
    "Alt+Click on a node and drag to another node to create an edge\n";
  Wt::WTextArea * const text_area =
    layout->addWidget( Wt::cpp14::make_unique< Wt::WTextArea >( help_message ),
      Wt::LayoutPosition::Center );
  text_area->setMinimumSize( 400, 100 );
}

void
EdgeWidget::construct_dummy_segment(
  Wt::WVBoxLayout * const outer_layout
){
  Wt::WContainerWidget * const container =
    outer_layout->addWidget( Wt::cpp14::make_unique< Wt::WContainerWidget >() );
  Wt::WBorderLayout * const layout =
    container->setLayout( Wt::cpp14::make_unique< Wt::WBorderLayout >() );
  layout->addWidget( Wt::cpp14::make_unique< Wt::WText >( "" ), Wt::LayoutPosition::Center )->setMinimumSize( 400, 200 );
}

}//namespace widgets
