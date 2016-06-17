// $Id: postfix_writer.cpp,v 1.53 2016/05/20 03:04:33 ist179719 Exp $ -*- c++ -*-
#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/declarations_counter.h"
#include "targets/postfix_writer.h"
#include "ast/all.h"  // all.h is automatically generated

//---------------------------------------------------------------------------
//     THIS IS THE VISITOR'S DEFINITION
//---------------------------------------------------------------------------

void zu::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
    bool save_seq = _1st_sequence;
    
    if(_1st_sequence == false)
        _1st_sequence = true;
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }

  if(save_seq == false){
        _externs_to_be.push_back("readi");
        _externs_to_be.push_back("readd");
        _externs_to_be.push_back("printi");
        _externs_to_be.push_back("printd");
        _externs_to_be.push_back("prints");
        _externs_to_be.push_back("println");
        process_externs();
   }
     
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
    if(_am_I_in_function){
        _pf.INT(node->value()); // push an integer
    }else{
        _pf.CONST(node->value());
    }
}

void zu::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
  int lbl1;

  if (_am_I_in_function) { 				//Contexto local
    _pf.RODATA();
    _pf.ALIGN();
    _pf.LABEL(mklbl(lbl1 = ++_lbl));
    _pf.DOUBLE(node->value());
    _pf.TEXT();
    _pf.ALIGN();
    _pf.ADDR(mklbl(lbl1));
    _pf.DLOAD();
  }
  else {																		//Contexto global
    _pf.DATA();
    _pf.ALIGN();
    _pf.LABEL(_id_to_label);
    _pf.DOUBLE(node->value());
  }
}

void zu::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.STR(node->value()); // output string characters

  if(!_am_I_in_function ){/* leave the address on the stack */ //local
    _pf.DATA();
    _pf.ALIGN();
    _pf.LABEL(node->value());
    _pf.ID(mklbl(lbl1));
  }else{
    _pf.TEXT(); // return to the TEXT segment
    _pf.ADDR(mklbl(lbl1)); // the string to be printed
      
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  
  node->left()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT && node->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.I2D();
  
  node->right()->accept(this, lvl);
  if(node->right()->type()->name() == basic_type::TYPE_INT && node->type()->name() == basic_type::TYPE_DOUBLE)
      _pf.I2D();
  
  if(node->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DADD();
  else    
    _pf.ADD();
}
void zu::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.SUB();
}
void zu::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MUL();
}
void zu::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.DIV();
}
void zu::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}
void zu::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LT();
}
void zu::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.LE();
}
void zu::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GE();
}
void zu::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.GT();
}
void zu::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.NE();
}
void zu::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.EQ();
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->lvalue()->accept(this, lvl);
  if(node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
    _pf.DLOAD();
  else
    _pf.LOAD();
}

//---------------------------------------------------------------------------

// void zu::postfix_writer::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
//   CHECK_TYPES(_compiler, _symtab, node);
//   // simplified generation: all variables are global
//   //_pf.ADDR(node->value()); // FIXME
// }

//---------------------------------------------------------------------------

void zu::postfix_writer::do_assignment_node(zu::assignment_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);

  // DAVID: horrible hack!
  // (this is caused by Zu not having explicit variable declarations)
//   const std::string &id = "i";//*(node->identifier()->identifier());
//   std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
//   if (symbol->value() == -1) {
//     _pf.DATA(); // variables are all global and live in DATA
//     _pf.ALIGN(); // make sure we are aligned
//     _pf.LABEL(id); // name variable location
//     _pf.CONST(0); // initialize it to 0 (zero)
//     _pf.TEXT(); // return to the TEXT segment
//     symbol->value(0);
//   }

  node->rvalue()->accept(this, lvl); // determine the new value // we nee to check doubles
  
  if(node->rvalue()->type()->name() !=  basic_type::TYPE_DOUBLE && node->lvalue()->type()->name() != basic_type::TYPE_DOUBLE) 
  	_pf.DUP();
  else if(node->rvalue()->type()->name() !=  basic_type::TYPE_DOUBLE && node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE)
  {
  	_pf.I2D();
  	_pf.DDUP();
  }
  else
  	_pf.DDUP();
  	
  node->lvalue()->accept(this, lvl); // store the value
  
  if(node->lvalue()->type()->name() != basic_type::TYPE_DOUBLE) 
  	_pf.STORE(); // store the value at address 
  else
  	_pf.DSTORE();
  
  
  
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_function_definition_node(zu::function_definition_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  std::string name = "";
  if(*(node->identifier()->identifier()) == "zu") name = "_main";
  else if(*(node->identifier()->identifier()) == "_main") name = "zu";
  else name = *(node->identifier()->identifier());
  
  delete_extern(name); //only removes if it exists
  // generate the main function (RTS mandates that its name be "_main")
  _pf.TEXT();
  _pf.ALIGN();
  if(node->visibility()) _pf.GLOBAL("_main", _pf.FUNC());
  _pf.LABEL(name);
  
  declarations_counter dc(_compiler, _symtab, _pf);
  
  node->accept(&dc, lvl);
  size_t declars_size = dc.get_size();
  
  size_t retsize = 0;
  if(node->return_type() != nullptr)
    retsize = node->return_type()->size();
  
  _pf.ENTER(declars_size); 
  
  _max_offset = declars_size ;
  _am_I_in_function = true;
  if(node->literal() != nullptr){
    node->literal()->accept(this, lvl+2);
    _pf.LOCA(-retsize);
  }
  _symtab.push(); //args
  
  _arg_offset =  8;
  if(node->args() != nullptr){
      _is_arg = true;
    node->args()->accept(this, lvl+2);
      _is_arg = false;
  }
  mklbl(_lbl_return = ++_lbl);
  node->body()->accept(this, lvl);

  
  _pf.LABEL(mklbl(_lbl_return));
  // end of function
    if(node->return_type() != nullptr){
        _pf.LOCAL(-retsize); //tirar o tipo de retorno (tamanho)
        if(node->return_type()->name() != basic_type::TYPE_DOUBLE){
            _pf.LOAD();
            _pf.POP();
        }else{ // doubles
            _pf.DLOAD();
            _pf.DPOP();
        }
   }
  _pf.LEAVE();
  _pf.RET();
  _max_offset = 0;
  
  _am_I_in_function = false;
  _arg_offset =  8;
  _symtab.pop(); //args

}

void zu::postfix_writer::do_function_invocation_node(zu::function_invocation_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
	
    std::shared_ptr<zu::symbol> symbol;
  
  std::string id;
  
  if(*(node->identifier()->identifier()) == "zu")
    id = std::string("_main");
  else if(*(node->identifier()->identifier()) == "_main")
    id = std::string("zu");
  else    
    id = *(node->identifier()->identifier());
    
  symbol = _symtab.find(*(node->identifier()->identifier()));
  int trash_size = 0;
	
  if(node->args() != nullptr){
	for(int i = (node->args()->size()) - 1; i >= 0; i--){
            cdk::expression_node * arg = (cdk::expression_node*) node->args()->node(i);
            arg->accept(this, lvl+1);
            trash_size += arg->type()->size();
        }
  }	
  _pf.CALL(id);
    if(trash_size > 0)				//Limpar os argumentos
        _pf.TRASH(trash_size);
    
    if(symbol->type() != nullptr){
        if(symbol->type()->name() == basic_type::TYPE_INT || symbol->type()->name() == basic_type::TYPE_POINTER
            || symbol->type()->name() ==  basic_type::TYPE_STRING)
            _pf.PUSH();
        else
            _pf.DPUSH();
    }
}

void zu::postfix_writer::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    CHECK_TYPES(_compiler, _symtab, node);
  

    _externs_to_be.push_back(*(node->identifier()->identifier()));


 
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value
  if(node->argument()->type()->name() != basic_type::TYPE_VOID)
      _pf.TRASH(node->argument()->type()->size());
}

void zu::postfix_writer::do_print_node(zu::print_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  
}

void zu::postfix_writer::do_println_node(zu::println_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl); // determine the value to print
  if (node->argument()->type()->name() == basic_type::TYPE_INT) {
    _pf.CALL("printi");
    _pf.TRASH(4); // delete the printed value
  }
  else if (node->argument()->type()->name() == basic_type::TYPE_STRING) {
    _pf.CALL("prints");
    _pf.TRASH(4); // delete the printed value's address
  }else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("printd");
    _pf.TRASH(8);
  }
  else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
  _pf.CALL("println"); // print a newline
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_read_node(zu::read_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  if (node->type()->name() == basic_type::TYPE_INT || node->type()->name() == basic_type::TYPE_UNSPEC) {
    _pf.CALL("readi");
    _pf.PUSH();
    
  }
  else if (node->type()->name() == basic_type::TYPE_DOUBLE) {
    _pf.CALL("readd");
    _pf.DPUSH();
    
  }
  else {
    throw std::string("The type expected for read_node to read is not supported.");
  }
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_for_node(zu::for_node * const node, int lvl) {
  CHECK_TYPES(_compiler, _symtab, node);
  int lbl1, lbl2, lbl3;
  if(node->init() != nullptr)
      node->init()->accept(this, lvl);
 
  _pf.LABEL(mklbl(lbl1 = ++_lbl));
  if(node->cond() != nullptr)
    node->cond()->accept(this, lvl);
  else
      (new cdk::integer_node(lvl, 1))->accept(this, lvl); 
  _pf.JZ(mklbl(lbl2 = ++_lbl));
  
  _break_lbls.push_back(mklbl(lbl2));
  _continue_lbls.push_back(mklbl(lbl3 = ++_lbl));
  
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl3));
  if(node->incr() != nullptr)
      node->incr()->accept(this, lvl);
  
  _pf.JMP(mklbl(lbl1));
  _pf.LABEL(mklbl(lbl2));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_node(zu::if_node * const node, int lvl) {
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void zu::postfix_writer::do_if_else_node(zu::if_else_node * const node, int lvl) {
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
    _pf.LABEL(mklbl(lbl1));
  if(node->elseblock() != nullptr){
    node->elseblock()->accept(this, lvl + 2);
  }
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//--------------------------------------------------------------------------

void zu::postfix_writer::do_and_node(zu::and_node * const node, int lvl){
  int lbl1;
  CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  _pf.DUP();                            // if it is false, then do not eval the next expression
  _pf.JZ(mklbl(lbl1 = ++_lbl));     
  
  node->right()->accept(this, lvl);
  _pf.AND();
  
  _pf.LABEL(mklbl(lbl1));
}


void zu::postfix_writer::do_not_node(zu::not_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl);
  _pf.NOT();
}

void zu::postfix_writer::do_or_node(zu::or_node * const node, int lvl){
 int lbl1;
    CHECK_TYPES(_compiler, _symtab, node);
  node->left()->accept(this, lvl);
  _pf.DUP();                            // if it is true, then do not eval the next expression
  _pf.JNZ(mklbl(lbl1 = ++_lbl));    
  node->right()->accept(this, lvl);
  _pf.OR();
  _pf.LABEL(mklbl(lbl1));
}

//--------------------------------------------------------------------------

void zu::postfix_writer::do_block_node(zu::block_node * const node, int lvl){
    _symtab.push();
    if(node->declarations() != nullptr)
        node->declarations()->accept(this, lvl);
    if(node->instructions() != nullptr)
        node->instructions()->accept(this, lvl);
    _symtab.pop();
}

void zu::postfix_writer::do_break_node(zu::break_node * const node, int lvl){
    std::string lbl = _break_lbls.back();
    _break_lbls.pop_back();
    _pf.JMP(lbl);
    
}

void zu::postfix_writer::do_continue_node(zu::continue_node * const node, int lvl){
    std::string lbl = _continue_lbls.back();
    _continue_lbls.pop_back();
    _pf.JMP(lbl);
}

void zu::postfix_writer::do_return_node(zu::return_node * const node, int lvl){
    _pf.JMP(mklbl(_lbl_return));
    
}

void zu::postfix_writer::do_identifier_l_node(zu::identifier_l_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    std::shared_ptr<zu::symbol> symbol = _symtab.find(*(node->identifier()));
    
    if(symbol->is_function()){
        _pf.LOCAL(-4); //return value
        
    }else if(!symbol->is_global()){
        _pf.LOCAL(symbol->offset());
        
    }else{
        _pf.ADDR(*(node->identifier()));
    
        
    }
    
}

void zu::postfix_writer::do_identity_node(zu::identity_node * const node, int lvl){
  CHECK_TYPES(_compiler, _symtab, node);
  node->argument()->accept(this, lvl);
}

void zu::postfix_writer::do_mem_alloc_node(zu::mem_alloc_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    node->total_elements()->accept(this, lvl + 2);
    _pf.INT(4); // size of integer
    _pf.MUL(); // size of integer * #posicoes
    _pf.ALLOC();
    _pf.SP();
}

void zu::postfix_writer::do_mem_index_node(zu::mem_index_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    node->base_addr()->accept(this, lvl + 2);// base
    _pf.LOAD();
    node->offset()->accept(this, lvl + 2); //offset
    if(node->type()->name() == basic_type::TYPE_DOUBLE)
        _pf.INT(8);
    else
        _pf.INT(4);
    _pf.MUL();  // offset * tamanho de inteiro
    _pf.ADD();  // base + offset * tamanho int
    
}

void zu::postfix_writer::do_mem_position_node(zu::mem_position_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    node->argument()->accept(this, lvl + 2);
}

void zu::postfix_writer::do_var_initialization_node(zu::var_initialization_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    std::string identifier = *(node->identifier()->identifier());
    std::shared_ptr<zu::symbol> symbol = _symtab.find(identifier);
    
        _id_to_label = identifier.c_str();
        if(node->expression()->type()->name() == basic_type::TYPE_STRING){
            
            node->expression()->accept(this, lvl + 2);
            _pf.TEXT();
        }else if(!_am_I_in_function){
            _pf.DATA();
            _pf.ALIGN();
            if(node->visibility())
                _pf.GLOBAL(identifier, _pf.OBJ());
        
            _pf.LABEL(identifier);
            node->expression()->accept(this, lvl + 2);
            if(node->type()->name() == basic_type::TYPE_DOUBLE && node->expression()->type()->name() == basic_type::TYPE_INT)		                                                      
                _pf.I2D();
            symbol->is_global(true);
            _pf.TEXT();
            _pf.ALIGN();
        
        }else{
            _pf.TEXT();
            
            if(node->visibility())
                _pf.GLOBAL(identifier, _pf.OBJ());
    
            symbol->offset(-_max_offset);
            _max_offset -= symbol->type()->size();
            node->expression()->accept(this, lvl + 2);
            if(node->type()->name() == basic_type::TYPE_DOUBLE && node->expression()->type()->name() == basic_type::TYPE_INT)			   _pf.I2D();
            _pf.LOCAL(symbol->offset());
            if(symbol->type()->name() == basic_type::TYPE_INT || symbol->type()->name() == basic_type::TYPE_POINTER)
                _pf.STORE();
            else if(symbol->type()->name() == basic_type::TYPE_DOUBLE)
                _pf.DSTORE();
                
            symbol->is_global(false);
        }
        
    
}

void zu::postfix_writer::do_var_declaration_node(zu::var_declaration_node * const node, int lvl){
    CHECK_TYPES(_compiler, _symtab, node);
    std::string identifier = *(node->identifier()->identifier());
    std::shared_ptr<zu::symbol> symbol = _symtab.find(identifier);
    
    if(node->ext()){
        _pf.EXTERN(identifier);
    }else if(!_am_I_in_function){
        if(node->visibility()){
            _pf.GLOBAL(identifier, _pf.OBJ());
            symbol->is_global(true);
        }
        _pf.BSS();
        _pf.ALIGN();
        _pf.LABEL(identifier);
        _pf.BYTE(node->type()->size());
        _pf.TEXT();
        _pf.ALIGN();
        
    }else{
        if(_is_arg){
            symbol->offset(_arg_offset);
            _arg_offset += symbol->type()->size();
            symbol->is_global(false);
        }else {
            symbol->offset(-_max_offset);
            _max_offset -= symbol->type()->size();
            symbol->is_global(false);
        }
    }
   
    
    
}