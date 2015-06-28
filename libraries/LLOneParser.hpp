#include <map>
#include <stack>
#include <set>
#include <vector>

#include <iostream>

namespace tul
{
  namespace libraries
  {
    template <typename SYMBOL>
    class LLOneParser
    {
    public:

      struct ParseReturn
      {
        const std::vector<SYMBOL> *child_symbols;
        bool is_error;
        bool was_removed;
      };

      ParseReturn parseSymbol(const SYMBOL &symbol_to_parse)
      {
        if (symbol_to_parse == symbol_stack.top())
        {
          symbol_stack.pop();
          return {nullptr, false, true};
        }

        typename std::map<SYMBOL, std::vector<SYMBOL>>::iterator iterator_over_map = production_rules[symbol_stack.top()].find(symbol_to_parse);
        if (iterator_over_map == production_rules[symbol_stack.top()].cend())
        {
          if (epsilonable_symbols.find(symbol_stack.top()) != epsilonable_symbols.cend())
          {
            symbol_stack.pop();
            return {nullptr, false, false};
          }
          return {nullptr, true, false};
        }

        const SYMBOL old_symbol = symbol_stack.top();
        symbol_stack.pop();
        for
        (
          std::size_t iterator_number = production_rules[old_symbol][symbol_to_parse].size() - 1;
          ;
          --iterator_number
        )
        {
          symbol_stack.push(production_rules[old_symbol][symbol_to_parse].at(iterator_number));
          if (iterator_number == 0)
            break;
        }
        return {&iterator_over_map->second, false, false};
      }

      bool addRule(const SYMBOL &left_side, const SYMBOL &transition_symbol, const std::vector<SYMBOL> &right_side)
      {
        typename std::map<SYMBOL, std::vector<SYMBOL>>::iterator iterator_over_map = production_rules[left_side].find(transition_symbol);
        if (iterator_over_map == production_rules[left_side].cend())
        {
          production_rules[left_side][transition_symbol] = right_side;
          return true;
        }
        return false;
      }

      void addEpsilon(const SYMBOL &left_side)
      {
        epsilonable_symbols.emplace(left_side);
      }

      void pushSymbol(const SYMBOL &symbol)
      {
        symbol_stack.emplace(symbol);
      }

    private:

      std::set<SYMBOL> epsilonable_symbols;
      std::stack<SYMBOL> symbol_stack;
      std::map<SYMBOL, std::map<SYMBOL, std::vector<SYMBOL>>> production_rules;

    };
  }
}
