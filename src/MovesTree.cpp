#include "MovesTree.h"

const Node ALLMOVES{
  new Node{
    new Node{ new Node{}, nullptr   , new Node{}, new Node{} }, 
    nullptr, 
    new Node{ new Node{}, new Node{}, new Node{}, nullptr    }, 
    new Node{ new Node{}, nullptr   , nullptr   , new Node{} }
  }, 
  new Node{
    nullptr, 
    new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
    new Node{ new Node{}, nullptr   , new Node{}, nullptr    }, 
    nullptr, 
    false
  }, 
  new Node{
    new Node{ new Node{}, nullptr   , new Node{}, new Node{} }, 
    new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
    new Node{ new Node{}, nullptr   , new Node{}, nullptr    }, 
    nullptr
  }, 
  new Node{
    new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
    nullptr, 
    nullptr, 
    nullptr, 
    false
  }, 
  false
};
