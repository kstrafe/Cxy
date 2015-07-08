#include "ConcreteSyntaxTree.hpp"


namespace tul
{
  namespace protocols
  {
    ConcreteSyntaxTree::~ConcreteSyntaxTree()
    {
      for (ConcreteSyntaxTree *child_ : children_)
        delete child_;
    }
  }
}
