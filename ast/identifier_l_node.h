#ifndef __ZU_IDENTIFIER_L_NODE_H__
#define __ZU_IDENTIFIER_L_NODE_H__

#include "lvalue_node.h"
#include <string>

namespace zu {

  /**
   * Class for describing syntactic tree leaves for holding identifier
   * values.
   */
  class identifier_l_node: public lvalue_node {
  	
  std::string *_identifier;
  
  public:
    inline identifier_l_node(int lineno, const char *s) :
        lvalue_node(lineno), _identifier(new std::string(s)) {
    }
    inline identifier_l_node(int lineno, const std::string &s) :
        lvalue_node(lineno), _identifier(new std::string(s)) {
    }
    inline identifier_l_node(int lineno, std::string *s) :
        lvalue_node(lineno), _identifier(s) {
    }

    inline std::string *identifier(){
	return _identifier;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_identifier_l_node(this, level);
    }

  };

} // zu

#endif
