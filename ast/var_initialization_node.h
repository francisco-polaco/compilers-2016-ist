#ifndef __ZU_VAR_INITIALIZATION_NODE_H__
#define __ZU_VAR_INITIALIZATION_NODE_H__

#include <cdk/ast/expression_node.h>
#include "identifier_l_node.h"

namespace zu {

  /**
   * Class for describing variable initialization nodes.
   */
  class var_initialization_node: public cdk::basic_node {
    
    bool _visibility; // !
    basic_type *_type;
    identifier_l_node *_identifier;
    cdk::expression_node *_expression;

  public:
    inline var_initialization_node(int lineno, bool visibility, basic_type* type, identifier_l_node *identifier, cdk::expression_node* expression) :
        cdk::basic_node(lineno), _visibility(visibility), _type(type), _identifier(identifier), _expression(expression) {
    }

    inline var_initialization_node(int lineno, bool visibility, basic_type* type, std::string *identifier, cdk::expression_node* expression) :
        cdk::basic_node(lineno), _visibility(visibility), _type(type), _identifier(new zu::identifier_l_node(lineno, identifier)), _expression(expression) {
    }

  public:
    
    inline bool visibility()
    {
      return _visibility;
    }
    
    inline basic_type* type()
    {
      return _type;
    }
    
    inline identifier_l_node *identifier()
    {
      return _identifier;
    }
    
    inline cdk::expression_node* expression()
    {
      return _expression;
    }
    

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_initialization_node(this, level);
    }

  };

} // zu

#endif 
