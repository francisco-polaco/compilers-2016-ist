#ifndef __ZU_FUNCTION_DECLARATION_NODE_H__
#define __ZU_FUNCTION_DECLARATION_NODE_H__

#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "identifier_l_node.h"


namespace zu {

  /**
   * Class for describing function_declare nodes
   */
   
  class function_declaration_node: public cdk::basic_node {
    bool _visibility;   // !
    bool _ext; // ?
    basic_type *_return_type;
    identifier_l_node *_identifier;
    cdk::sequence_node *_args;
    cdk::expression_node *_literal;

  public:
    inline function_declaration_node(int lineno, bool visibility, bool ext, basic_type *return_type, 
      identifier_l_node *identifier, cdk::sequence_node *args, cdk::expression_node *literal) :
       cdk::basic_node(lineno), _visibility(visibility), _ext(ext), _return_type(return_type),
       _identifier(identifier), _args(args), _literal(literal){}
       
    inline function_declaration_node(int lineno, bool visibility, bool ext, basic_type *return_type, 
    std::string *identifier, cdk::sequence_node *args, cdk::expression_node *literal) :
      cdk::basic_node(lineno), _visibility(visibility), _ext(ext), _return_type(return_type),
       _identifier(new identifier_l_node(lineno, identifier)), _args(args), _literal(literal){}

    inline bool visibility() {
      return _visibility;
    }
    inline bool ext() {
      return _ext;
    }
    inline basic_type *return_type() {
      return _return_type;
    }
    inline identifier_l_node *identifier() {
      return _identifier;
    }
     inline cdk::sequence_node *args() {
      return _args;
    }
    inline cdk::expression_node *literal(){
      return _literal;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_declaration_node(this, level);
    }

  };

} // zu

#endif
