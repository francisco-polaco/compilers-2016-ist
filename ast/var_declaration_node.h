#ifndef __ZU_VAR_DECLARATION_NODE_H__
#define __ZU_VAR_DECLARATION_NODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

  /**
   * Class for describing variable declaration nodes.
   */
  class var_declaration_node: public cdk::basic_node {
    bool _visibility; // !
    bool _ext; // ?
    basic_type* _type;
    identifier_l_node* _identifier;

  public:
    inline var_declaration_node(int lineno, bool visibility, bool ext, basic_type* type, std::string* identifier) :
        cdk::basic_node(lineno), _visibility(visibility), _ext(ext), _type(type), _identifier(new identifier_l_node(lineno, identifier)) {
    }
    inline var_declaration_node(int lineno, bool visibility, bool ext, basic_type* type, identifier_l_node* identifier) :
        cdk::basic_node(lineno), _visibility(visibility), _ext(ext), _type(type), _identifier(identifier) {
    }

  public:
    
    inline bool visibility()
    {
      return _visibility;
    }
    
    inline bool ext()
    {
      return _ext;
    }
    
    inline basic_type* type()
    {
      return _type;
    }
    
    inline identifier_l_node* identifier()
    {
      return _identifier;
    }
    

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_var_declaration_node(this, level);
    }

  };

} // zu

#endif 

