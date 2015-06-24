#include "ActionGenerator.hpp"
#include "protocols/Action.hpp"
#include "protocols/Type.hpp"

#include "libraries/Mealy.hpp"

#include <cstddef>

namespace tul
{
  namespace lexer
  {
    namespace dependency
    {
      protocols::Action ActionGenerator::computeAction(protocols::Type type)
      {
        return machine.transist(type, 5);
      }

      ActionGenerator::ActionGenerator()
      {
        /*
          n = no action
          p = push current symbol on to the current token
          tx = transfer something of type x
          tg = transfer a grouper token
          tr = transfer a string token
          ts = transfer a symbol token
          ta = transfer an alnumu token
          txp = transfer x and then push
          ptx = push and then transfer
        */
        machine.setTransitionTable(table);
      }

      using namespace protocols;

      constexpr const tul::library::Mealy<std::size_t, protocols::Action, protocols::Type>::Compound ActionGenerator::table[25]
        =
          { /*    alphanamu  grouper     quote    symbol    whitespace*/
            /*0*/ {1, Action::p},    {0, Action::ptg},   {2, Action::n},  {4, Action::p},   {0, Action::n},
            /*1*/ {1, Action::p},    {0, Action::taptg}, {2, Action::ta}, {4, Action::tap}, {0, Action::ta},
            /*2*/ {2, Action::p},    {2, Action::p},     {3, Action::n},  {2, Action::p},   {2, Action::p},
            /*3*/ {1, Action::trp},  {0, Action::trptg}, {2, Action::p},  {4, Action::trp}, {0, Action::tr},
            /*4*/ {1, Action::tsp},  {0, Action::tsptg}, {2, Action::ts}, {4, Action::p},   {0, Action::ts}
          };
    }
  }
}
