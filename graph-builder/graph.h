// vim:sw=3:expandtab:shiftround
#pragma once

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>

template<typename VertexT, typename WeightT>
class graph
{
   using vertex_list_t = std::vector<VertexT>;
   using vertex_index_t = typename vertex_list_t::size_type;

   struct EdgeInfo {
      vertex_index_t dest_vertex_index;
      WeightT weight;
   };

   using edge_list_t = std::vector<EdgeInfo>;
   using adjacency_list_t = std::vector<edge_list_t>;

private:
   vertex_index_t add_vertex(const VertexT& vert)
   {
      auto x = std::find( m_vertex_list.begin(), m_vertex_list.end(), vert );

      if(x == m_vertex_list.end())
      {
         m_vertex_list.emplace_back(vert);
         m_adjacency_list.emplace_back(edge_list_t{});

         return m_vertex_list.size() - 1;
      }

      return x - m_vertex_list.begin();
   }

   template<class visitor_t, class visitor1_t>
   void traverse_by_depth_from(vertex_index_t index, visitor_t& v, visitor1_t& v1 )
   {
      v( m_vertex_list[index] );

      for( auto einfo : m_adjacency_list[index] )
      {
         v1( m_vertex_list[index], m_vertex_list[einfo.dest_vertex_index], einfo.weight );

         traverse_by_depth_from( einfo.dest_vertex_index, v, v1 );
      }
   }

public:
   void add_edge(const VertexT& src, const VertexT& dest, const WeightT& weight)
   {
      auto num_vertices = m_vertex_list.size();

      auto src_pos = add_vertex(src);
      auto dest_pos = add_vertex(dest);

      if(num_vertices != m_vertex_list.size())
         m_adjacency_list[src_pos].emplace_back(EdgeInfo{dest_pos, weight});
   }

   template<class visitor_t, class visitor1_t>
   void traverse_by_depth(visitor_t v, visitor1_t v1)
   {
      for( vertex_index_t index = 0; index < m_vertex_list.size(); ++index )
      {
         traverse_by_depth_from( index, v, v1 );
      }
   }

private:
   vertex_list_t m_vertex_list;
   adjacency_list_t m_adjacency_list;
};
