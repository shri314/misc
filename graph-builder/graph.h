// vim:sw=3:expandtab:shiftround
#pragma once

template<typename VertexT, typename WeightT>
class graph
{
   using vertex_list_t = std::unordered_set<VertexT>;

   struct AdjacencyInfo {
      vertex_list_t::iterator src_index;
      vertex_list_t::iterator dest_index;
      WeightT weight;
   }

   vertex_list_t m_vertex_list;
   std::vector<AdjacencyInfo> m_adjacency_list;

public:
   void add_edge(const VertexT& src, const VertexT& dest, const WeightT& weight)
   {
      auto src_index = m_vertex_list.find(src);
      auto dest_index = m_vertex_list.find(dest);

      m_adjacency_list.emplace_back(src_index, dest_index, weight);
   }

   template<visitor_t>
   void traverse_by_depth(visitor_t v)
   {
   }
};



int main()
{
   graph<std::string, int> g;

   g.add_edge("a", "b", 1);
   g.add_edge("a", "e", 1);

   
}
