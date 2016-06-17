// $Id: continue_node.h,v 1.5 2016/03/16 22:47:31 ist179719 Exp $ -*- c++ -*-
#ifndef __ZU_CONTINUE_NODE_H__
#define __ZU_CONTINUE_NODE_H__

#include <cdk/ast/basic_node.h>

namespace zu {


   /**
   * Class for describing continue nodes.
   */
  class continue_node: public cdk::basic_node {

  public:
    inline continue_node(int lineno) : basic_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_continue_node(this, level);
    }

  };

} // zu

#endif
