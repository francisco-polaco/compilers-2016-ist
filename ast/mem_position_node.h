#ifndef __ZU_MEM_POSITION_NODE_H__
#define __ZU_MEM_POSITION_NODE_H__

#include <cdk/ast/unary_expression_node.h>

namespace zu {

  /**
   * Class for describing memory position nodes.
   */
  class mem_position_node: public cdk::unary_expression_node {

  public:
    inline mem_position_node(int lineno, lvalue_node *arg) :
        unary_expression_node(lineno, arg) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_position_node(this, level);
    }

  };

} // zu

#endif

