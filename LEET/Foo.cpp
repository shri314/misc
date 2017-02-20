#include <cassert>
#include <memory>
#include <string>
#include <vector>

class Solution
{
private:
   struct match_result
   {
      bool matched;
      bool consumed_fully;
      bool consumed;
   };

   struct char_parser_base
   {
      virtual match_result match(char x) = 0;
      virtual match_result match()       = 0;
   };

   struct any_char_parser : char_parser_base
   {
      virtual match_result match(char x) override { return match_result{true, true, true}; }
      virtual match_result match() override { return match_result{false, true, true}; }
   };

   struct fixed_char_parser : char_parser_base
   {
      const char m_fixed_char;

      explicit fixed_char_parser(char fixed_char)
         : m_fixed_char(fixed_char)
      {
      }

      virtual match_result match(char x) override { return match_result{x == m_fixed_char, true, true}; }
      virtual match_result match() override { return match_result{false, true, true}; }
   };

   struct repeat_parser : char_parser_base
   {
      std::unique_ptr<char_parser_base> const m_other_parser;

      explicit repeat_parser(std::unique_ptr<char_parser_base> other_parser)
         : m_other_parser(std::move(other_parser))
      {
      }

      virtual match_result match(char x) override
      {
         auto&& result = m_other_parser->match(x);

         return match_result{true, false, result.matched};
      }

      virtual match_result match() override
      {
         auto&& result = m_other_parser->match();

         return match_result{true, false, result.matched};
      }
   };

   auto Compile(std::string expression)
   {
      std::vector<std::unique_ptr<char_parser_base>> parsers;

      for (auto&& x : expression)
      {
         switch (x)
         {
            case '.':
               parsers.push_back(std::make_unique<any_char_parser>());
               break;

            case '*':
               if (!parsers.empty())
               {
                  auto&& new_p = std::make_unique<repeat_parser>(std::move(parsers.back()));

                  parsers.back() = std::move(new_p);
               }
               else
                  throw "bad expression, nothing before *";

               break;

            default:
               parsers.push_back(std::make_unique<fixed_char_parser>(x));
               break;
         }
      }

      return parsers;
   }

public:
   bool isMatch(std::string s, std::string p)
   {
      auto&& parsers = Compile(p);

      auto&& cur_parser = parsers.begin();
      auto&& end_parser = parsers.end();

      auto&& cur_c = s.begin();
      auto&& end_c = s.end();

      while (true)
      {
         if (cur_parser != end_parser)
         {
            auto&& result = cur_c != end_c ? (*cur_parser)->match(*cur_c) : (*cur_parser)->match();

            if (!result.consumed)
            {
               ++cur_parser;   // send unconsumed c to the next parser
               continue;
            }

            if (cur_c != end_c) ++cur_c;

            if (!result.matched) return false;

            if (result.consumed_fully) ++cur_parser;
         }
         else
            break;
      }

      return cur_c == end_c && cur_parser == end_parser;
   }
};


int main()
{
   Solution s;

   assert(s.isMatch("abc", "abc") == true);
   assert(s.isMatch("abc", "abcd") == false);
   assert(s.isMatch("aac", "a*c") == true);
   assert(s.isMatch("aac", "...") == true);
   assert(s.isMatch("abcdef", ".*") == true);
   assert(s.isMatch("abcdef", ".*c") == false);
   assert(s.isMatch("abcdef", ".*c") == false);


   assert(s.isMatch("aa", "a") == false);
   assert(s.isMatch("aa", "aa") == true);
   assert(s.isMatch("aaa", "aa") == false);
   assert(s.isMatch("aa", "a*") == true);
   assert(s.isMatch("aa", ".*") == true);
   assert(s.isMatch("ab", ".*") == true);
   assert(s.isMatch("aab", "c*a*b") == true);
   assert(s.isMatch("aaa", "a*a") == true);
}
