#pragma once

#include <memory>
#include <Wt/WColor>

/*
Helpful links:
https://coolors.co
https://www.canva.com/learn/website-color-schemes/
*/

namespace view {

/*struct Color {
  Color( uint16_t r, uint16_t g, uint16_t b ) :
    R( r ),
    G( g ),
    B( b )
  {}

  uint16_t R;
  uint16_t G;
  uint16_t B;
};*/

struct ColorTheme {
  virtual Wt::WColor background() const = 0;
  virtual Wt::WColor grid_line() const = 0;

  virtual Wt::WColor intermediate_node() const = 0;
  virtual Wt::WColor final_node() const = 0;
  virtual Wt::WColor edge() const = 0;
  virtual Wt::WColor selection_outline() const = 0;
};

using ColorThemeSP = std::shared_ptr< ColorTheme >;
using ColorThemeCSP = std::shared_ptr< const ColorTheme >;
using ColorThemeUP = std::unique_ptr< ColorTheme >;
using ColorThemeCUP = std::unique_ptr< const ColorTheme >;

struct Theme1 : public ColorTheme {
  // https://coolors.co/f6f9f4-eff4ed-58b5a3-1e5c6f-171717
  Wt::WColor background() const override { return Wt::WColor( 241, 247, 239 ); }//F6F9F4
  Wt::WColor grid_line() const override { return Wt::WColor( 133, 150, 158 ); }//EFF4ED

  Wt::WColor intermediate_node() const override { return Wt::WColor( 88, 181, 163 ); }//58B5A3
  Wt::WColor final_node() const override { return Wt::WColor( 30, 92, 111 ); }//1E5C6F
  Wt::WColor edge() const override { return Wt::WColor( 23, 23, 23 ); }//171717
  Wt::WColor selection_outline() const override { return Wt::WColor( 23, 23, 23 ); }//171717
};


};
