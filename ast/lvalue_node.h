// $Id: lvalue_node.h,v 1.5 2016/04/14 16:55:28 ist179719 Exp $
#ifndef __ZU_NODE_EXPRESSION_LEFTVALUE_H__
#define __ZU_NODE_EXPRESSION_LEFTVALUE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */

   // Isto tem de deixar de herdar de simple_value_node??
   // Sim, porque esse tipo de nos so guarda inteiros, doubles e strings. E no zu pode guardar funcoes e outras cenas
  class lvalue_node: public cdk::expression_node{

  public:
    inline lvalue_node(int lineno) :
        cdk::expression_node(lineno) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    virtual void accept(basic_ast_visitor *sp, int level) = 0;

  };

} // zu

#endif
