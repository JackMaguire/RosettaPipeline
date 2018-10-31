#include <view/GraphWidget.hh>
#include <global_data/options.hh>

#include <graph/Graph.hh>
#include <graph/Edge.hh>
#include <graph/Node.hh>

#include <Wt/WLength.h>
#include <Wt/WGlobal.h>
#include <Wt/WMessageBox.h>

#include <iostream>
#include <math.h>
#include <memory>

namespace view {

GraphWidget::GraphWidget(
  graph::GraphSP const & graph,
  GraphToolbarWidget * toolbar
) :
  WPaintedWidget(),
  graph_( graph ),
  theme_( std::make_unique< Theme1 >() ),
  toolbar_( toolbar ),
  width_( 500 ),
  height_( 600 )
{
  //TODO
  //node_label_font_.setFamily( Wt::FontFamily::Monospace, "'Courier New'" );

  setLayoutSizeAware( true );

  resize( width_, height_ ); // Provide a default size.
  setMinimumSize( width_, height_ );
  setMaximumSize( width_, height_ );

  init_listeners();
  init_painting_tools();
}

GraphWidget::~GraphWidget(){}

void
GraphWidget::init_listeners(){
  node_is_currently_being_dragged_ = false;
  edge_is_currently_being_created_ = false;
  shift_was_down_when_most_recent_object_was_selected_ = false;

  mouseWentDown().connect( this, & GraphWidget::mouseDown );
  mouseWentUp().connect( this, & GraphWidget::mouseReleased );
  mouseDragged().connect( this, & GraphWidget::mouseDraggedImpl );
}

void
GraphWidget::init_painting_tools(){
  default_pen_.setWidth( 2 );
  selected_edge_pen_.setWidth( 4 );
}

GraphInteraction
GraphWidget::determineInteractionType( Wt::WMouseEvent const & e ) const {
  //first check keyboard shortcuts
  bool const shift_is_down = e.modifiers().test( Wt::KeyboardModifier::Shift );
  bool const alt_is_down = e.modifiers().test( Wt::KeyboardModifier::Alt );
  if( shift_is_down ){
    return GraphInteraction::DELETE;
  } else if( alt_is_down ){
    return GraphInteraction::ADD;
  }

  assert( toolbar_ );
  return toolbar_->currentInteraction();
}

void
GraphWidget::mouseDown( Wt::WMouseEvent const & e ) {
  Wt::Coordinates c = e.widget();
  auto const x = c.x;
  auto const y = c.y;
  last_mouse_press_x_ = x;
  last_mouse_press_y_ = y;

  GraphInteraction const interaction_type = determineInteractionType( e );

  switch( interaction_type ){
  case GraphInteraction::SELECT:
  case GraphInteraction::DELETE:
    for( auto const & node : graph_->nodes() ) {
      if( hitbox_for_node_.at( node ).pointIsInBox( x, y ) ) {
	graph_->setSelectedNode( node );
	shift_was_down_when_most_recent_object_was_selected_ = ( interaction_type == GraphInteraction::DELETE );
	node_is_currently_being_dragged_ = true;
	update();
	return;
      }
    }

    for( auto const & edge : graph_->edges() ) {
      if( hitbox_for_edge_.at( edge ).pointIsInBox( x, y ) ) {
	graph_->setSelectedEdge( edge );
	shift_was_down_when_most_recent_object_was_selected_ = ( interaction_type == GraphInteraction::DELETE );
	update();
      }
    }
    break;
  case GraphInteraction::ADD:
    for( auto const & node : graph_->nodes() ) {
      if( hitbox_for_node_.at( node ).pointIsInBox( x, y ) ) {
	graph_->setSelectedNode( node );
	edge_is_currently_being_created_ = true;
	graph_->setGhostEdge( std::make_shared< graph::PreliminaryEdge >( node, x, y ) );
	update();
	return;
      }
    }
    break;
  }
}

void
GraphWidget::mouseReleased( Wt::WMouseEvent const & e ) {
  Wt::Coordinates c = e.widget();
  auto const x = c.x;
  auto const y = c.y;

  GraphInteraction const interaction_type = determineInteractionType( e );

  switch( interaction_type ){
  case GraphInteraction::SELECT:
    if( node_is_currently_being_dragged_ ) {
      node_is_currently_being_dragged_ = false;
      if( abs( x - last_mouse_press_x_ ) > 4 || abs( y - last_mouse_press_y_ ) > 4 ) {
	graph_->selectedNode()->setX( getClosestPointForPoint( x ) );
	graph_->selectedNode()->setY( getClosestPointForPoint( y ) );
	update();
      }
      return;
    }
    break;
  case GraphInteraction::ADD:
    if( edge_is_currently_being_created_ ) {
      edge_is_currently_being_created_ = false;
      graph_->setGhostEdge( 0 );
      // Check to see if xy corresponds to a node
      for( auto const & node : graph_->nodes() ) {
	if( node == graph_->selectedNode() )
	  continue;
	if( hitbox_for_node_.at( node ).pointIsInBox( x, y ) ) {
	  auto const & new_edge = graph_->addEdge( graph_->selectedNode(), node );
	  if( new_edge ){
	    graph_->setSelectedEdge( new_edge );
	  }
	  break;
	}
      }
    } else {
      // create new node
      auto const x2 = getClosestPointForPoint( x );
      auto const y2 = getClosestPointForPoint( y );
      graph_->addNode( std::make_shared< graph::Node >( x2, y2 ) );
    }
    update();
    break;
  case GraphInteraction::DELETE:
    if( shift_was_down_when_most_recent_object_was_selected_ ){
      auto & selected_node = graph_->selectedNode();
      if( selected_node != 0 ) {
	if( graph_->getNumNodes() > 1 ) {// Don't want an empty graph
	  if( hitbox_for_node_.at( selected_node ).pointIsInBox( x, y ) ) {
	    Wt::WMessageBox * const messageBox = addChild(
	      Wt::cpp14::make_unique< Wt::WMessageBox >(
		"Delete",
		"<p>Permanently delete node with title " + selected_node->title() + "?</p>",
		Wt::Icon::Warning, Wt::StandardButton::Yes | Wt::StandardButton::No)
	    );
	    messageBox->setModal(false);
	    messageBox->buttonClicked().connect(
	      [=] {
		if( messageBox->buttonResult() == Wt::StandardButton::Yes) {
		  graph_->removeNodeAndDeleteItsEdges( selected_node );
		  graph_->setSelectedNode( graph_->nodes()[ 0 ] );
		  update();
		}
		this->removeChild(messageBox);
	      }
	    );
	    messageBox->show();
	  }
	  return;
	}
      } else if( graph_->selectedEdge() != 0 ) {
	if( hitbox_for_edge_.at( graph_->selectedEdge() ).pointIsInBox( x, y ) ) {
	  /*final Object[] options = { "Yes, delete",
	    "No, don't delete" };
	    int n = JOptionPane.showOptionDialog( new JFrame(),
	    "Delete Selected Edge?",
	    "Delete?",
	    JOptionPane.YES_NO_OPTION,
	    JOptionPane.QUESTION_MESSAGE,
	    null,
	    options,
	    options[ 1 ] );
	    if( n == 1 )
	    return;*/
	  auto & selected_edge = graph_->selectedEdge();
	  graph_->removeEdgeAndNotifyItsNodes( selected_edge );
	  graph_->setSelectedNode( graph_->nodes()[ 0 ] );
	  update();
	}
      }
      return;
    }
    break;
  }

}

void
GraphWidget::mouseDraggedImpl( Wt::WMouseEvent const & e ) {
  Wt::Coordinates c = e.widget();
  auto const x = c.x;
  auto const y = c.y;

  if( node_is_currently_being_dragged_ ) {
    auto const & selected_node = graph_->selectedNode();
    selected_node->setX( getClosestPointForPoint( x ) );
    selected_node->setY( getClosestPointForPoint( y ) );
    update();
    return;
  }

  if( edge_is_currently_being_created_ ) {
    graph_->ghostEdge()->cursor_x = x;
    graph_->ghostEdge()->cursor_y = y;
    update();
    return;
  }
}

void
GraphWidget::paintEvent( Wt::WPaintDevice * paintDevice ) {
  Wt::WPainter painter( paintDevice );
  painter.setPen( default_pen_ );
  int const grid_size = global_data::Options::grid_size;

  //Outline
  painter.setBrush( Wt::WBrush( theme_->grid_line() ) );
  //top left
  painter.drawLine( 0, 0, 0, 5 );
  painter.drawLine( 0, 0, 5, 0 );

  //top right
  painter.drawLine( width_, 0, width_, 5 );
  painter.drawLine( width_, 0, width_ - 5, 0 );

  //bottom right
  painter.drawLine( width_, height_, width_, height_ - 5 );
  painter.drawLine( width_, height_, width_ - 5, height_ );

  //bottom left
  painter.drawLine( 0, height_, 0, height_ - 5 );
  painter.drawLine( 0, height_, 5, height_ );

  /*

  //Background
  //
  painter.setBrush( Wt::WBrush( theme_->background() ) );
  painter.drawRect( 0, 0, width_, height_ );

  //Grid
  //
  painter.setBrush( Wt::WBrush( theme_->grid_line() ) );
  for( int x = -1; x < width_; x += 2 * grid_size ) {
    painter.drawLine( x, 0, x, height_ );
  }
  for( int y = -1; y < height_; y += 2 * grid_size ) {
    painter.drawLine( 0, y, width_, y );
  }

  */

  //Edges
  //
  painter.setBrush( Wt::WBrush( theme_->edge() ) );
  int const edge_offset = ( node_width_ - ( node_width_ / 2 ) ) * grid_size;
  for( auto const & edge_sp : graph_->edges() ){
    drawEdge( edge_sp, painter, grid_size, edge_offset );
  }
  painter.setPen( default_pen_ );

  //Ghost Edge
  //
  graph::PreliminaryEdgeCSP ghost_edge = graph_->ghostEdge();
  if( ghost_edge ){
    drawGhostEdge( ghost_edge, painter, grid_size, edge_offset );
  }

  //Nodes
  //
  node_label_font_.setSize( Wt::WLength( 3 * grid_size ) );
  painter.setFont( node_label_font_ );
  int const selection_width = grid_size / 2;
  for( auto const & node_sp : graph_->nodes() ){
    drawNode( node_sp, painter, grid_size, selection_width );
  }
}

void
GraphWidget::drawNode(
  graph::NodeSP const & node,
  Wt::WPainter & painter,
  int const grid_size,
  int const selection_width
) {
  int const x = node->X() * grid_size + ( grid_size / 2 );
  int const y = node->Y() * grid_size + ( grid_size / 2 );
  int const diameter = grid_size * 3;
  
  if( node == graph_->selectedNode() ){
    painter.setBrush( Wt::WBrush( theme_->selection_outline() ) );
    int const sx = x - selection_width;
    int const sy = y - selection_width;
    int const sdiameter = diameter + 2 * selection_width;
    painter.drawEllipse( sx, sy, sdiameter, sdiameter );
  }

  if( node->numDownstreamEdges() == 0 ) {
    painter.setBrush( Wt::WBrush( theme_->final_node() ) );
  } else {
    painter.setBrush( Wt::WBrush( theme_->intermediate_node() ) );
  }

  painter.drawEllipse( x, y, diameter, diameter );
  hitbox_for_node_[ node ] = hitbox( x, y, diameter, diameter );

  if( global_data::Options::show_node_titles ) {
    //g2D.setColor( Color.black );
    if( global_data::Options::put_node_titles_to_side ) {
      painter.drawText(
	x + diameter + selection_width,
	y,
	100,//width
	100,//height
	Wt::WFlags< Wt::AlignmentFlag >(),
	node->title()
      );
    } else {
      painter.drawText(
	x,
	y - selection_width * 4,
	100,//width
	100,//height
	Wt::WFlags< Wt::AlignmentFlag >(),
	node->title()
      );
    }
  }
}



void
GraphWidget::drawEdge(
  graph::EdgeSP const & edge,
  Wt::WPainter & painter,
  int const grid_size,
  int const offset
) {
  graph::Node const & n_from = edge->sourceNode();
  graph::Node const & n_to = edge->destinationNode();

  if( edge == graph_->selectedEdge() ) {
    painter.setPen( selected_edge_pen_ );
  } else {
    painter.setPen( default_pen_ );
  }

  // Draw main line
  int const source_x = n_from.X() * grid_size + offset;
  int const source_y = n_from.Y() * grid_size + offset;
  int const dest_x = n_to.X() * grid_size + offset;
  int const dest_y = n_to.Y() * grid_size + offset;
  painter.drawLine( source_x, source_y, dest_x, dest_y );

  // Draw Arrow Heads
  // B is halfway point between two nodes
  double const Bx = grid_size * ( n_to.X() + n_from.X() ) / 2 + offset;
  double const By = grid_size * ( n_to.Y() + n_from.Y() ) / 2 + offset;
  double const arrow_length = grid_size;

  double const theta_in_radians = atan2( double(source_y) - By, double(source_x) - Bx );

  {//copied from https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line
    int const x1 = (int) Bx;
    int const y1 = (int) By;
    int const x2 = int( Bx + arrow_length );
    int const y2 = int( By - arrow_length );
    int const dx = x2-x1;
    int const dy = y2-y1;
    int const nx = dx * cos( theta_in_radians ) - dy * sin( theta_in_radians ) + x1;
    int const ny = dx * sin( theta_in_radians ) + dy * cos( theta_in_radians ) + y1;
    painter.drawLine( x1, y1, nx, ny );
  }

  {//copied from https://gamedev.stackexchange.com/questions/121478/how-to-rotate-a-2d-line
    int const x1 = (int) Bx;
    int const y1 = (int) By;
    int const x2 = int( Bx + arrow_length );
    int const y2 = int( By + arrow_length );
    int const dx = x2-x1;
    int const dy = y2-y1;
    int const nx = dx * cos( theta_in_radians ) - dy * sin( theta_in_radians ) + x1;
    int const ny = dx * sin( theta_in_radians ) + dy * cos( theta_in_radians ) + y1;
    painter.drawLine( x1, y1, nx, ny );
  }

  hitbox_for_edge_[ edge ] = hitbox(
    (int) ( Bx - grid_size ),
    (int) ( By - grid_size ),
    grid_size * 2,
    grid_size * 2
  );

}

void
GraphWidget::drawGhostEdge(
  graph::PreliminaryEdgeCSP const & ghost_edge,
  Wt::WPainter & painter,
  int const grid_size,
  int const offset
) {
  graph::NodeSP const & n_from = ghost_edge->source_node;
  int const source_x = n_from->X() * grid_size + offset;
  int const source_y = n_from->Y() * grid_size + offset;
  painter.drawLine( source_x, source_y, ghost_edge->cursor_x, ghost_edge->cursor_y );
}

//TODO this needs a better name
int
GraphWidget::getClosestPointForPoint( int point ) const {
  int const grid_size = global_data::Options::grid_size;
  int const offset = ( node_width_ - ( node_width_ / 2 ) ) * grid_size;
  double const new_point = ( point - offset ) / double( grid_size );
  return int( round( new_point ) );
}

}//namespace view
