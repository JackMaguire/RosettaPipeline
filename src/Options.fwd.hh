#pragma once

struct Options;

using OptionsSP = std::shared_ptr< Options >;
using OptionsCSP = std::shared_ptr< const Options >;
using OptionsWP = std::weak_ptr< Options >;
using OptionsCWP = std::weak_ptr< const Options >;

class OptionsHolder;
