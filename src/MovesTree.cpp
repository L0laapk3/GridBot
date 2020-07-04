#include "MovesTree.h"

const Node ALLMOVES{
  new Node{
    new Node{
      new Node{
        new Node{ nullptr   , nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, new Node{}, new Node{}, nullptr   , false }, 
        new Node{ new Node{}, new Node{}, nullptr   , new Node{}, false }
      }, 
      nullptr, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , new Node{}, new Node{}, nullptr   , false }, 
        new Node{ new Node{}, new Node{}, new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , nullptr   , nullptr   , new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, new Node{}, nullptr   , new Node{}, false }
      }
    }, 
    nullptr, 
    new Node{
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, new Node{}, new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, new Node{}, false }, 
        new Node{ new Node{}, new Node{}, new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , nullptr   , new Node{}, new Node{}, false }, 
        new Node{ new Node{}, new Node{}, new Node{}, nullptr   , false }, 
        nullptr
      }, 
      nullptr
    }, 
    new Node{
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }, 
        false
      }, 
      nullptr, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }
    }
  }, 
  new Node{
    nullptr, 
    new Node{
      nullptr, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        nullptr, 
        false
      }, 
      false
    }, 
    new Node{
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr
      }, 
      nullptr
    }, 
    new Node{
      new Node{
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        nullptr, 
        nullptr, 
        false
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      nullptr, 
      new Node{
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        nullptr, 
        false
      }, 
      false
    }, 
    false
  }, 
  new Node{
    new Node{
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }, 
      nullptr, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        nullptr, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }
    }, 
    new Node{
      nullptr, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , nullptr   , false }, 
        false
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , nullptr   , nullptr   , false }, 
        false
      }, 
      false
    }, 
    new Node{
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        nullptr, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , new Node{}, false }
      }, 
      new Node{
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      new Node{
        new Node{ new Node{}, nullptr   , new Node{}, new Node{}, false }, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        new Node{ new Node{}, nullptr   , nullptr   , nullptr   , false }, 
        nullptr
      }, 
      nullptr
    }, 
    nullptr
  }, 
  new Node{
    new Node{
      new Node{
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      nullptr, 
      new Node{
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
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
    }, 
    new Node{
      nullptr, 
      new Node{
        nullptr, 
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      new Node{
        nullptr, 
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      nullptr, 
      false
    }, 
    nullptr, 
    new Node{
      new Node{
        nullptr, 
        nullptr, 
        new Node{ nullptr   , nullptr   , new Node{}, nullptr   , false }, 
        nullptr, 
        false
      }, 
      nullptr, 
      nullptr, 
      nullptr, 
      false
    }, 
    false
  }, 
  false
};
