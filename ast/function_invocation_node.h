#ifndef __ZU_FUNCTION_INVOC_NODE_H__
#define __ZU_FUNCTION_INVOC_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <string>
#include "identifier_l_node.h"

namespace zu {

  /**
   * Class for describing function_invocation nodes
   */
  class function_invocation_node: public cdk::expression_node {
    cdk::sequence_node *_args;    
    identifier_l_node *_identifier;


  public:
    inline function_invocation_node(int lineno, identifier_l_node *identifier, cdk::sequence_node *args) :
       cdk::expression_node(lineno),  _args(args), _identifier(identifier) {
    }
    inline function_invocation_node(int lineno, std::string *identifier, cdk::sequence_node *args) :
       cdk::expression_node(lineno),  _args(args), _identifier(new identifier_l_node(lineno, identifier)) {
    }
    inline identifier_l_node *identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *args() {
      return _args;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_invocation_node(this, level);
    }

  };

} // zu

#endif
