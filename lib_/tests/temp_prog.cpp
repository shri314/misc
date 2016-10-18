#include <iostream>

int Fib(int x)
{
   if(x <= 1)
      return x;

   return Fib(x - 1) + Fib(x - 2);
}

template<class T>
class List
{
   struct node;
public:
   List()
      : m_head(0)
   {
   }

   void push_back(const T& x)
   {
      push_back_node( new node(x) );
   }

   void print() const
   {
      std::string sep;
      const node* cur = m_head;
      while(cur != 0)
      {
         std::cout << sep << cur->x;
         cur = cur->next;
         sep = ", ";
      }

      std::cout << std::endl;
   }

   void swap(List& rhs)
   {
      std::swap(m_head, rhs.m_head);
   }

   void reverse()
   {
      List new_list;

      while( node* top = pop_front_node() )
         new_list.push_front_node(top);

      swap(new_list);
   }

   ~List()
   {
      while( node* top = pop_front_node() )
         delete top;
   }

private:
   struct node
   {
      T x;
      node* next;

      node(const T& x)
         : x(x)
         , next(0)
      {
      }
   };

private:
   void push_back_node(node* n)
   {
      node** next_ptr_ptr = &m_head;
      while(*next_ptr_ptr != 0)
         next_ptr_ptr = &((*next_ptr_ptr)->next);

      *next_ptr_ptr = n;
   }

   node* pop_front_node()
   {
      node* ret = m_head;

      if(m_head)
         m_head = m_head->next;

      return ret;
   }

   void push_front_node(node* n)
   {
      n->next = m_head;
      m_head = n;
   }

private:
   node* m_head;
};

int main(int argc,char *argv[])
{
   List<int> L;

   for(int i = 0; i < 9; ++i)
      L.push_back( Fib(i) );

   L.print();

   L.reverse();

   L.print();

   return 0;
}

