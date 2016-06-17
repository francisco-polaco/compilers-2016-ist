// $Id: read_node.h,v 1.4 2016/05/12 23:59:38 ist179719 Exp $ -*- c++ -*-
#ifndef __ZU_READNODE_H__
#define __ZU_READNODE_H__


namespace zu {

  /**
   * Class for describing read nodes.
   */
  class read_node: public cdk::expression_node {
 

  public:
    inline read_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_read_node(this, level);
    }

  };

} // zu

#endif
