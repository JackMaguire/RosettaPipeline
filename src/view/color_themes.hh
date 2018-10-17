#pragma once

/*
Helpful links:
https://coolors.co
https://www.canva.com/learn/website-color-schemes/
*/

namespace view {

struct Color {
  Color( uint16_t r, uint16_t g, uint16_t b ) :
    R( r ),
    G( g ),
    B( b )
  {}

  uint16_t R;
  uint16_t G;
  uint16_t B;
};

struct ColorTheme {
  virtual Color background() const = 0;
  virtual Color grid_line() const = 0;

  virtual Color intermediate_node() const = 0;
  virtual Color final_node() const = 0;
  virtual Color edge() const = 0;
  virtual Color selection_outline const = 0;
};

struct Theme1 : public ColorTheme {
  Color background() const override { return Color( 241, 247, 239 ) };//F1F7EF
  Color grid_line() const override { return Color( 133, 150, 158 ) };//85969E

  Color intermediate_node() const override { return Color( 88, 181, 163 ) };//58B5A3
  Color final_node() const override { return Color( 30, 92, 111 ) };//1E5C6F
  Color edge() const override { return Color( 23, 23, 23 ) };//171717
  Color selection_outline() const override { return Color( 23, 23, 23 ) };//171717
}


};
