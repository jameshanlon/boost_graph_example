#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <boost/lexical_cast.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/dynamic_property_map.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/iteration_macros.hpp>

enum VertexType {
  A, B, C, NONE
};

// Bundled vertex property map.
struct VertexProperties {
  int id;
  VertexType type;
  std::string name;
};

using Graph = boost::adjacency_list<boost::vecS,
                                    boost::vecS,
                                    boost::bidirectionalS,
                                    VertexProperties,
                                    boost::no_property>;

namespace boost {
  // Lexical casts to/from enums for property map.
  template<>
  VertexType lexical_cast(const std::string &s) {
    if (s == "A") return VertexType::A;
    if (s == "B") return VertexType::B;
    if (s == "C") return VertexType::C;
    return VertexType::NONE;
  }
  template<>
  std::string lexical_cast(const VertexType &t) {
    if (t == VertexType::A) return "A";
    if (t == VertexType::B) return "B";
    if (t == VertexType::C) return "C";
    return "NONE";
  }
} // End namespace boost.

int main(int argc, char *argv[]) {
  Graph graph;
  // Setup dynamic property map.
  boost::dynamic_properties dp;
  dp.property("id",   boost::get(&VertexProperties::id,   graph));
  dp.property("type", boost::get(&VertexProperties::type, graph));
  dp.property("name", boost::get(&VertexProperties::name, graph));
  // Read graph from file.
  std::fstream infile("test.dot");
  if (!infile.is_open()) {
    std::cout << "Could not open file";
    return 1;
  }
  if (!boost::read_graphviz(infile, graph, dp, "name")) {
    std::cout << "Could not read file";
    return 1;
  }
  // Loop over all vertices and print properties.
  BGL_FORALL_VERTICES(v, graph, Graph) {
    std::cout << "VERTEX [id="<<graph[v].id
                      <<" name="<<graph[v].name
                      <<" type="<<boost::lexical_cast<std::string>(graph[v].type)
                      <<"]\n";
  }
  // Loop over all edges.
  BGL_FORALL_EDGES(e, graph, Graph) {
    std::cout << "EDGE " << boost::source(e, graph) << " -> "
                         << boost::target(e, graph) << "\n";
  }
  // Get vertices by index.
  for (boost::graph_traits<Graph>::vertices_size_type i = 0;
       i < boost::num_vertices(graph); ++i) {
    boost::graph_traits<Graph>::vertex_descriptor
        vd = boost::vertex(i, graph);
    std::cout << "VERTEX_BY_INDEX " << graph[vd].name << "\n";
  }
  // Write graphviz file.
  boost::write_graphviz_dp(std::cout, graph, dp, std::string("name"));
  return 0;
}
