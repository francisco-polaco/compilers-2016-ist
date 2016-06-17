// $Id: not_node.h,v 1.5 2016/03/16 22:47:31 ist179719 Exp $ -*- c++ -*-
#ifndef __ZU_NOT_NODE_H__
#define __ZU_NOT_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  /**
   * Class for describing not nodes.
   */  

  class not_node: public cdk::unary_expression_node {
  
  public:
    /**
     * @param lineno source code line number for this node
     * @param left first opernot
     * @param right second opernot
     */
    inline not_node(int lineno, cdk::expression_node *arg) :
        cdk::unary_expression_node(lineno, arg) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_not_node(this, level);
    }

  };

} // zu

#endif
