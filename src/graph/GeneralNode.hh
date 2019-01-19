#pragma once

#include <graph/GeneralNode.fwd.hh>
#include <graph/Node.hh>
#include <graph/Edge.fwd.hh>
#include <graph/Graph.fwd.hh>

#include <serialization.hh>

#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include <Options.hh>

namespace graph {

class GeneralNode : public Node {

friend class Graph;

public:
  GeneralNode(
    Options const &,
    int x,
    int y,
    Graph * parent
  );

  GeneralNode(
    Options const &,
    std::string const & title,
    int x,
    int y,
    Graph * parent
  );

  //load ctor
  GeneralNode(
    serialization::Unarchiver &,
    Options const &,
    Graph * parent
  );

  ~GeneralNode();

  std::string
  uniqueToken() const override {
    return "GEN_NODE";
  }

public://getters and setters:
  std::string const & getBashScript() const {
    return bash_script_;
  }

  void setBashScript( std::string const & script ){
    bash_script_ = script;
  }

public://general compilation access
  void addToRunScript(
    Options const & options,
    std::stringstream & run_script
  ) const override;

protected://saving and loading
  void save_additional_tokens( serialization::Archiver & archiver ) const override;

  void
  load_from_token(
    std::string const & token,
    std::string const & value
  ) override;


private:
  std::string bash_script_;
};

}//namespace graph
