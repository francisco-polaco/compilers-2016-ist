#ifndef __ZU_FUNCTION_DEFINITION_NODE_H__
#define __ZU_FUNCTION_DEFINITION_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "identifier_l_node.h"


namespace zu {

  /**
   * Class for describing function_definition nodes.
   */
  class function_definition_node: public cdk::basic_node {
    identifier_l_node *_identifier;
    bool _visibility;   /* ! */
    basic_type *_return_type;
    cdk::sequence_node *_args;
    cdk::expression_node *_literal;
    block_node *_body;

  public:
    inline function_definition_node(int lineno, bool visibility, basic_type *return_type, 
      identifier_l_node *identifier, cdk::sequence_node *args, cdk::expression_node *literal, block_node *body) :
       cdk::basic_node(lineno), _identifier(identifier), _visibility(visibility), 
       _return_type(return_type), _args(args), _literal(literal), _body(body){}
    
    inline function_definition_node(int lineno, bool visibility, basic_type *return_type, 
      std::string *identifier, cdk::sequence_node *args, cdk::expression_node *literal, block_node *body) :
       cdk::basic_node(lineno), _identifier(new identifier_l_node(lineno, identifier)), _visibility(visibility), 
       _return_type(return_type), _args(args), _literal(literal), _body(body){}
    
  public:

    inline basic_type *return_type() {
      return _return_type;
    }

    inline block_node *body() {
      return _body;
    }
    inline identifier_l_node *identifier() {
      return _identifier;
    }
    inline bool visibility() {
      return _visibility;
    }

    inline cdk::sequence_node *args() {
      return _args;
    }
    inline cdk::expression_node *literal(){
      return _literal;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_definition_node(this, level);
    }

  };

} // zu

#endif
