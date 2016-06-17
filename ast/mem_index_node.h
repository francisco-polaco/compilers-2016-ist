#ifndef __ZU_MEM_INDEX_NODE_H__
#define __ZU_MEM_INDEX_NODE_H__

#include "lvalue_node.h"

namespace zu {

  /**
   * Class for describing memory index nodes.
   */
  class mem_index_node: public lvalue_node {
  	cdk::expression_node *_base_addr;
  	cdk::expression_node *_offset;

  public:
    inline mem_index_node(int lineno, cdk::expression_node *base_addr, cdk::expression_node *offset) :
        lvalue_node(lineno), _base_addr(base_addr), _offset(offset) {
    }

    inline cdk::expression_node *base_addr(){
    	return _base_addr;
    }

    inline cdk::expression_node *offset(){
    	return _offset;
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mem_index_node(this, level);
    }

  };

} // zu

#endif

