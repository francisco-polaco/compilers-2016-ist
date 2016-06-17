// $Id: break_node.h,v 1.4 2016/03/16 22:47:31 ist179719 Exp $ -*- c++ -*-
#ifndef __ZU_BREAK_NODE_H__
#define __ZU_BREAK_NODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {

   /**
   * Class for describing break nodes.
   */

  class break_node: public cdk::basic_node {
   

  public:
    inline break_node(int lineno) : basic_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_break_node(this, level);
    }

  };

} // zu

#endif
