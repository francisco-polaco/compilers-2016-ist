// $Id: println_node.h,v 1.2 2016/03/16 22:47:31 ist179719 Exp $ -*- c++ -*-
#ifndef __ZU_PRINTLN_NODE_H__
#define __ZU_PRINTLN_NODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing println nodes.
   */
  class println_node: public cdk::basic_node {
    cdk::expression_node *_argument;

  public:
    inline println_node(int lineno, cdk::expression_node *argument) :
        cdk::basic_node(lineno), _argument(argument) {
    }

  public:
    inline cdk::expression_node *argument() {
      return _argument;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_println_node(this, level);
    }

  };

} // zu

#endif
