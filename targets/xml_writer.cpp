// $Id: xml_writer.cpp,v 1.49 2016/05/20 02:16:33 ist179100 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void zu::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++){
    if(node->node(i) != nullptr)
      node->node(i)->accept(this, lvl + 2);
  }
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}

void zu::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
    openTag("Double", lvl);
    processSimple(node, lvl);
    closeTag("Double", lvl);
}

void zu::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  if(node->left() != nullptr)
      node->left()->accept(this, lvl + 2);
  if(node->right() != nullptr)
      node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void zu::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

// void zu::xml_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
//   CHECK_TYPES(_compiler, _symtab, node);
//   //processSimple(node, lvl);
//   openTag(node, lvl);
//   closeTag(node, lvl);
// }

//---------------------------------------------------------------------------

void zu::xml_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_function_invocation_node(zu::function_invocation_node * const node, int lvl) {
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  openTag("arguments", lvl + 2);
  if(node->args() != nullptr)
    node->args()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  // FIXME
  openTag(node, lvl);
  openTag("identifier", lvl + 2);
  if(node->identifier() != nullptr)
    node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  openTag("visibility", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->visibility() <<  std::endl;
  closeTag("visibility", lvl + 2);
  if(node->return_type() != nullptr){
    openTag("return_type type=" + convert_types(node->return_type()), lvl + 2);
    closeTag("return_type", lvl + 2);
  }
  openTag("arguments", lvl + 2);
  if(node->args() != nullptr)
    node->args()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  openTag("literal", lvl + 2);
  if(node->literal() != nullptr)
    node->literal()->accept(this, lvl + 4);
  closeTag("literal", lvl + 2);
  openTag("body", lvl + 2);
  if(node->body() != nullptr)
    node->body()->accept(this, lvl + 4);
  closeTag("body", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
  // FIXME
  openTag(node, lvl);
  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  openTag("visibility", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->visibility() <<  std::endl;
  closeTag("visibility", lvl + 2);
  openTag("external", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->ext() <<  std::endl;
  closeTag("external", lvl + 2);
  if(node->return_type() != nullptr){
    openTag("return_type type=" + convert_types(node->return_type()), lvl + 2);
    closeTag("return_type", lvl + 2);
  }
  openTag("arguments", lvl + 2);
  if(node->args() != nullptr)
      node->args()->accept(this, lvl + 4);
  closeTag("arguments", lvl + 2);
  openTag("literal", lvl + 2);
  if(node->literal() != nullptr)
      node->literal()->accept(this, lvl + 4);
  closeTag("literal", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}


void zu::xml_writer::do_println_node(zu::println_node * const node, int lvl) {
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_read_node(zu::read_node * const node, int lvl) {
  openTag(node, lvl);
  node->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_for_node(zu::for_node * const node, int lvl) {
  // FIXME
  openTag(node, lvl);
  openTag("initialization", lvl + 2);
  if(node->init() != nullptr)
    node->init()->accept(this, lvl + 4);
  closeTag("initialization", lvl + 2);
  openTag("condition", lvl + 2);
  if(node->cond() != nullptr)
    node->cond()->accept(this, lvl + 4);
  else
    (new cdk::integer_node(lvl, 1))->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("increment", lvl + 2);
  if(node->incr() != nullptr)
    node->incr()->accept(this, lvl + 4);
  closeTag("increment", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void zu::xml_writer::do_if_node(zu::if_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("then", lvl + 2);
  node->thenblock()->accept(this, lvl + 4);
  closeTag("then", lvl + 2);
  openTag("else", lvl + 2);
  if(node->elseblock() != nullptr)
      node->elseblock()->accept(this, lvl + 4);
  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}

//-------------------------------------------------------------------- 

void zu::xml_writer::do_and_node(zu::and_node * const node, int lvl){
  // FIXME
  processBinaryExpression(node, lvl);
}

//-------------------------------------------------------------------- 

void zu::xml_writer::do_or_node(zu::or_node * const node, int lvl){
  // FIXME
  processBinaryExpression(node, lvl);
}


void zu::xml_writer::do_not_node(zu::not_node * const node, int lvl){
  // FIXME
  processUnaryExpression(node, lvl);
}

//-------------------------------------------------------------------- 

void zu::xml_writer::do_block_node(zu::block_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  openTag("declarations", lvl + 2);
  if(node->declarations() != nullptr)
      node->declarations()->accept(this, lvl + 4);
  closeTag("declarations", lvl + 2);
  openTag("instructions", lvl + 2);
  if(node->instructions() != nullptr)
      node->instructions()->accept(this, lvl + 4);
  closeTag("instructions", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_break_node(zu::break_node * const node, int lvl){
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_continue_node(zu::continue_node * const node, int lvl){
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_return_node(zu::return_node * const node, int lvl){
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  closeTag(node, lvl);
}

void zu::xml_writer::do_identifier_l_node(zu::identifier_l_node * const node, int lvl){
  // FIXME
    openTag(node , lvl);
    os() << std::string(lvl, ' ') << *(node->identifier()) <<  std::endl;
    closeTag(node, lvl);
}

void zu::xml_writer::do_identity_node(zu::identity_node * const node, int lvl){
  // FIXME
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_mem_alloc_node(zu::mem_alloc_node * const node, int lvl){
  // FIXME
  CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  openTag("total_elements", lvl + 2);
  node->total_elements()->accept(this, lvl + 4);
  closeTag("total_elements", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_mem_index_node(zu::mem_index_node * const node, int lvl){
  // FIXME
    openTag(node, lvl);
    openTag("base address", lvl + 2);
    node->base_addr()->accept(this, lvl + 4);
    closeTag("base address", lvl + 2);
    openTag("offset", lvl + 2);
    node->offset()->accept(this, lvl + 4);
    closeTag("offset", lvl + 2);
    closeTag(node, lvl);
}

void zu::xml_writer::do_mem_position_node(zu::mem_position_node * const node, int lvl){
  // FIXME 
  processUnaryExpression(node, lvl);
}

void zu::xml_writer::do_var_initialization_node(zu::var_initialization_node * const node, int lvl){
  // FIXME 
  openTag(node, lvl);
  openTag("visibility", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->visibility() <<  std::endl;
  closeTag("visibility", lvl + 2);
  openTag("type type=" + convert_types(node->type()), lvl + 2);
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  openTag("expression", lvl + 2);
  node->expression()->accept(this, lvl + 4);
  closeTag("expression", lvl + 2);
  closeTag(node, lvl);
}

void zu::xml_writer::do_var_declaration_node(zu::var_declaration_node * const node, int lvl){
  // FIXME 
  openTag(node, lvl);
  openTag("visibility", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->visibility() <<  std::endl;
  closeTag("visibility", lvl + 2);
  openTag("external", lvl + 2);
  os() << std::string(lvl + 4, ' ') << node->ext() <<  std::endl;
  closeTag("external", lvl + 2);
  openTag("type type=" + convert_types(node->type()), lvl + 2);
  closeTag("type", lvl + 2);
  openTag("identifier", lvl + 2);
  node->identifier()->accept(this, lvl + 4);
  closeTag("identifier", lvl + 2);
  closeTag(node, lvl);
}

std::string zu::xml_writer::convert_types(basic_type *t){
    if(t->name() == basic_type::TYPE_INT){
        return "Integer";
    }else if(t->name() == basic_type::TYPE_DOUBLE){
        return "Double";
    }else if(t->name() == basic_type::TYPE_STRING){
        return "String";
    }else if(t->name() == basic_type::TYPE_POINTER){
        return "Pointer";
    }else
        return nullptr;
}
