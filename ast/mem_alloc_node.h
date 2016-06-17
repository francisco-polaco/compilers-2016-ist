#ifndef __ZU_MEM_ALLOC_NODE_H__
#define __ZU_MEM_ALLOC_NODE_H__

#include <cdk/ast/expression_node.h>

namespace zu {

	/**
   * Class for describing memory allocation nodes.
   */

  class mem_alloc_node: public cdk::expression_node {
  	cdk::expression_node *_total_elements;

  public:
    inline mem_alloc_node(int lineno, cdk::expression_node *total_elements) :
        cdk::expression_node(lineno), _total_elements(total_elements){ }

  public:

  	inline cdk::expression_node *total_elements(){
    	return _total_elements;
    }



    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_alloc_node(this, level);
    }

  };

} // zu

#endif
