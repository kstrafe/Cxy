#pragma once

#include <cstddef>

namespace tul
{
  namespace library
  {
    /**
      Implement a table-based mealy machine.
      The table is given as an array of Compound.
      This array is only in one dimension, but will be spliced into pieces by
      the "argument size" parameter that's given to transist .

      The table is to be constructed like the following example:
      const tul::Mealy<int, Action, EntryType>::Compound table[]
      {
          {1, push},            {2, push},            {3, push},             {0, transfer},
          {1, push},            {1, push},            {0, transferWord},     {0, transferWord},
          {0, transferInteger}, {2, push},            {0, transferInteger},  {0, transferInteger},
          {3, push},            {3, push},            {4, nothing},          {3, push},
          {0, transferString},  {0, transferString},  {3, push},             {0, transferString},
      };

      Where Action and EntryType are enumerable items.
      The next invocation on transist will be:
      EntryType t;
      transist(t, 4);
    */
    template <typename STATE = unsigned, typename ACTION = unsigned, typename INPUT = unsigned>
    class Mealy
    {
    public:

      struct Compound
      {
        STATE new_state;
        ACTION action_out;
      };

      ACTION transist (const INPUT input_element, const STATE alphabet_size)
      {
        Compound compound
          = transition_table
          [
            static_cast<std::size_t>(current_state)
            * static_cast<std::size_t>(alphabet_size)
            + static_cast<std::size_t>(input_element)
          ];
        current_state = compound.new_state;
        return compound.action_out;
      }

      void setState(const STATE state_)
      {
        current_state = state_;
      }

      STATE getState() const
      {
        return current_state;
      }

      void setTransitionTable(const Compound *action_table)
      {
        transition_table = action_table;
      }

    private:

      const Compound *transition_table;
      STATE current_state = 0;
    };
  } // namespace library
} // namespace tul
