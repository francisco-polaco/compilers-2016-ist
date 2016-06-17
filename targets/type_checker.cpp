// $Id: type_checker.cpp,v 1.56 2016/05/20 14:38:01 ist179719 Exp $ -*- c++ -*-
#include <string>
#include <vector>
#include <memory>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void zu::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
   ASSERT_UNSPEC;
   node->type(new basic_type(4, basic_type::TYPE_INT));
}

void zu::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
   ASSERT_UNSPEC;
   node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}

void zu::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
   ASSERT_UNSPEC;
   node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void zu::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
    node->argument()->accept(this, lvl + 2);
    if (node->argument()->type()->name() == basic_type::TYPE_INT)
	node->type(new basic_type(4, basic_type::TYPE_INT));

    else if (node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
	node->type(new basic_type(8, basic_type::TYPE_DOUBLE));

    else
	throw std::string("wrong type in argument of unary expression");
}

void zu::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
   processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void zu::type_checker::processBinaryExpressionInts(cdk::binary_expression_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    if (node->left()->type()->name() == basic_type::TYPE_INT){
	if (node->right()->type()->name() == basic_type::TYPE_INT){
            node->type(new basic_type(4, basic_type::TYPE_INT));
	}
    }
    else
	throw std::string("wrong type in left argument of binary expression");
}

inline void zu::type_checker::processBinaryExpressionIntDouble(cdk::binary_expression_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    if (node->left()->type()->name() == basic_type::TYPE_INT){
	if (node->right()->type()->name() == basic_type::TYPE_INT){
            node->type(new basic_type(4, basic_type::TYPE_INT));
	}
	else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE){
            node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
	}
    }
    else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE){
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    }
    else
	throw std::string("wrong type in left argument of binary expression");
}

inline void zu::type_checker::processBinaryExpressionIntDoublePointer(cdk::binary_expression_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    if (node->left()->type()->name() == basic_type::TYPE_INT){
	if (node->right()->type()->name() == basic_type::TYPE_INT){
            node->type(new basic_type(4, basic_type::TYPE_INT));
	}
	else if (node->right()->type()->name() == basic_type::TYPE_DOUBLE){
            node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
	}
	else if (node->right()->type()->name() == basic_type::TYPE_POINTER){
            node->type(new basic_type(4, basic_type::TYPE_POINTER));
	}
    }
    else if (node->left()->type()->name() == basic_type::TYPE_DOUBLE){
        node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
    }
    else if (node->left()->type()->name() == basic_type::TYPE_POINTER){
	if (node->right()->type()->name() == basic_type::TYPE_INT){
            node->type(new basic_type(4, basic_type::TYPE_POINTER));
	}
	else if (node->right()->type()->name() == basic_type::TYPE_POINTER){
            node->type(new basic_type(4, basic_type::TYPE_POINTER));
	}
    }
    else
     throw std::string("wrong type in left argument of binary expression");
}


//---------------------------------------------------------------------------

void zu::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
   processBinaryExpressionIntDoublePointer(node, lvl);
}
void zu::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
   processBinaryExpressionIntDoublePointer(node, lvl);
}
void zu::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpressionInts(node, lvl);
}
void zu::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpressionIntDouble(node, lvl);
}
void zu::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpressionIntDoublePointer(node, lvl);
}
void zu::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpressionIntDoublePointer(node, lvl);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_rvalue_node(zu::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

//---------------------------------------------------------------------------

// void zu::type_checker::do_lvalue_node(zu::lvalue_node * const node, int lvl) {
//   const std::string &id = NULL;// FIXME node->value();
//   std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
//   if (symbol == nullptr) throw id + " undeclared";
//   // hackish stuff...
//   node->type(new basic_type(4, basic_type::TYPE_INT));
// }

//---------------------------------------------------------------------------

void zu::type_checker::do_assignment_node(zu::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;

  // DAVID: horrible hack!
  // (this is caused by Zu not having explicit variable declarations)
  // const std::string &id = NULL;// FIXME node->lvalue()->value();
  // if (!_symtab.find(id)) {
  //   _symtab.insert(id, std::make_shared<zu::symbol>(new basic_type(4, basic_type::TYPE_INT), id, -1)); // put in the symbol table
  // }
 
   node->lvalue()->accept(this, lvl + 2);
   node->rvalue()->accept(this, lvl + 2);

   if (node->lvalue()->type()->name() == basic_type::TYPE_INT && node->rvalue()->type()->name() == basic_type::TYPE_INT)
	node->type(new basic_type(4, basic_type::TYPE_INT));

   else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_DOUBLE)
	node->type(new basic_type(4, basic_type::TYPE_DOUBLE));

   else if (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE && node->rvalue()->type()->name() == basic_type::TYPE_INT)
	node->type(new basic_type(4, basic_type::TYPE_DOUBLE));

   else if (node->lvalue()->type()->name() == basic_type::TYPE_STRING && node->rvalue()->type()->name() == basic_type::TYPE_STRING)
	node->type(new basic_type(4, basic_type::TYPE_STRING));

   else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER && node->rvalue()->type()->name() == basic_type::TYPE_POINTER)
	node->type(new basic_type(4, basic_type::TYPE_POINTER));
   else if (node->lvalue()->type()->name() == basic_type::TYPE_POINTER )
	node->type(new basic_type(4, basic_type::TYPE_INT));

   else
     
	node->type(new basic_type(4, basic_type::TYPE_UNSPEC));
   
}

//---------------------------------------------------------------------------

void zu::type_checker::do_function_invocation_node(zu::function_invocation_node * const node, int lvl) {
   ASSERT_UNSPEC;

   const std::string &id = *(node->identifier()->identifier());
   std::shared_ptr<zu::symbol> symbol = _symtab.find(id/*, context_depth*/);
       
   if (symbol) {
	const std::vector<basic_type*> arguments = symbol->args();
    	cdk::expression_node *argmt;

	if(node->args()!=NULL){
            if(arguments.size() != node->args()->size())
                throw id + ": invalid number of arguments";
                 
            for (unsigned int i = 0; (i < arguments.size()  &&  i < node->args()->size()); i++) {
		argmt = (cdk::expression_node*) node->args()->node(i);
		node->args()->node(i)->accept(this, lvl);

	 	if(argmt->type()->name() == basic_type::TYPE_UNSPEC  && arguments.at(i)->name() == basic_type::TYPE_INT){
			argmt->type(new basic_type(4, basic_type::TYPE_INT));
			continue;
	    	}
	    	else if(argmt->type()->name() == basic_type::TYPE_UNSPEC  && arguments.at(i)->name() == basic_type::TYPE_DOUBLE){
			argmt->type(new basic_type(8, basic_type::TYPE_DOUBLE));
			continue;
	    	}
		else if (argmt->type()->name() != arguments.at(i)->name())
			;//throw id + ": unmatching argument types";
	    }
	}
   }
   else if((symbol = _symtab.find(id)) && ! (symbol->is_function()))
	throw id + " is not a function";
   else
	throw id + " undeclared";
   
   if(symbol->type() == nullptr)
       node->type(new basic_type(0, basic_type::TYPE_VOID));
   else node->type(symbol->type());
  

}

void zu::type_checker::do_function_definition_node(zu::function_definition_node * const node, int lvl) {

    const std::string &id = *(node->identifier()->identifier());
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
    if(symbol == nullptr || !symbol->is_function()){ 
        std::vector<basic_type*> arguments;
        if(node->args() != nullptr){
            for(size_t i = 0; i < node->args()->size(); i++){
                zu::var_declaration_node *argument = (zu::var_declaration_node *)node->args()->node(i);
                arguments.push_back(argument->type());
            }
        }
        std::shared_ptr<zu::symbol> sym = std::make_shared<zu::symbol>(node->return_type(), id, arguments, true, -1);
        if(node->literal() != nullptr){
            node->literal()->accept(this, lvl + 2);
            sym->return_value(node->literal());
        }
        if(_symtab.insert(id, sym) == false)
            std::cout<< "definition insuccess" << std::endl;
        
    }else if(!symbol->is_function_defined()){
        std::vector<basic_type*> arguments;
        if(node->args() != nullptr){
            for(size_t i = 0; i < node->args()->size(); i++){
                zu::var_declaration_node *argument = (zu::var_declaration_node *)node->args()->node(i);
                arguments.push_back(argument->type());
            }
        }
        
        
        if(node->literal() != nullptr){
            node->literal()->accept(this, lvl + 2);
        }
        std::shared_ptr<zu::symbol> sym = std::make_shared<zu::symbol>(node->return_type(), id, arguments, true, -1);
        
        if(_symtab.replace(id, sym) == false)
            std::cout<< "redefinition insuccess" << std::endl;
        
    }else{
        throw std::string("function already def");
    }
    
}

void zu::type_checker::do_function_declaration_node(zu::function_declaration_node * const node, int lvl) {
    
 const std::string &id = *(node->identifier()->identifier());
  if(_symtab.find(id) == nullptr){
      std::vector<basic_type*> arguments;
        if(node->args() != nullptr){
            for(size_t i = 0; i < node->args()->size(); i++){
                zu::var_declaration_node *argument = (zu::var_declaration_node *)node->args()->node(i);
                arguments.push_back(argument->type());
            }
        }
     _symtab.insert(id, std::make_shared<zu::symbol>(node->return_type(), id, arguments, false, -1));
      
  }
}

//---------------------------------------------------------------------------

void zu::type_checker::do_evaluation_node(zu::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

void zu::type_checker::do_print_node(zu::print_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
	throw std::string("pointers cannot be printed");
}

void zu::type_checker::do_println_node(zu::println_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() == basic_type::TYPE_POINTER)
	throw std::string("pointers cannot be printed");
}

//---------------------------------------------------------------------------

void zu::type_checker::do_read_node(zu::read_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(new basic_type(4, basic_type::TYPE_UNSPEC)); // we dont know what to expect
}

//---------------------------------------------------------------------------

void zu::type_checker::do_for_node(zu::for_node * const node, int lvl) {
   if (node->cond() != nullptr)
	node->cond()->accept(this, lvl + 4);

   else
	(new cdk::integer_node(lvl, 1))->accept(this, lvl + 4); //FIXME leaks de memoria	
}

//---------------------------------------------------------------------------

void zu::type_checker::do_if_node(zu::if_node * const node, int lvl) {
   node->condition()->accept(this, lvl + 4);
   if(node->condition()->type()->name() != basic_type::TYPE_INT)
	throw std::string("if node condition must be of type integer");
}

void zu::type_checker::do_if_else_node(zu::if_else_node * const node, int lvl) {
   node->condition()->accept(this, lvl + 4);
   if(node->condition()->type()->name() != basic_type::TYPE_INT)
	throw std::string("if else node condition must be of type integer");
}

//---------------------------------------------------------------------------

void zu::type_checker::do_and_node(zu::and_node * const node, int lvl){
  processBinaryExpressionInts(node, lvl);
}

void zu::type_checker::do_or_node(zu::or_node * const node, int lvl){
  processBinaryExpressionInts(node, lvl);
}

void zu::type_checker::do_not_node(zu::not_node * const node, int lvl){
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void zu::type_checker::do_block_node(zu::block_node * const node, int lvl){
    _symtab.push();
    _symtab.pop();
    
}

void zu::type_checker::do_break_node(zu::break_node * const node, int lvl){
    
}

void zu::type_checker::do_continue_node(zu::continue_node * const node, int lvl){
}

void zu::type_checker::do_return_node(zu::return_node * const node, int lvl){
    
}

void zu::type_checker::do_identifier_l_node(zu::identifier_l_node * const node, int lvl){
    ASSERT_UNSPEC;
    const std::string &id  = *(node->identifier());
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
    if(symbol == nullptr) throw id + " not found";
    node->type(symbol->type());
    
}

void zu::type_checker::do_identity_node(zu::identity_node * const node, int lvl){
    processUnaryExpression(node, lvl);

}

void zu::type_checker::do_mem_alloc_node(zu::mem_alloc_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->total_elements()->accept(this, lvl + 4);
  if(node->total_elements()->type()->name() != basic_type::TYPE_INT)
	throw std::string("memory allocation node expect an argument of type integer");
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void zu::type_checker::do_mem_index_node(zu::mem_index_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->base_addr()->accept(this, lvl + 2);
  if (node->base_addr()->type()->name() != basic_type::TYPE_POINTER)
    throw std::string("wrong type in argument of index node");
  
  
  node->offset()->accept(this, lvl + 2);
  
  if(node->offset()->type()->name() == basic_type::TYPE_UNSPEC)
    node->offset()->type(new basic_type(4, basic_type::TYPE_INT));
  
  if (node->offset()->type()->name() !=basic_type::TYPE_INT)
    throw std::string("wrong type in argument of index node");
  
  node->type(new basic_type(4, basic_type::TYPE_INT));
    
}

void zu::type_checker::do_mem_position_node(zu::mem_position_node * const node, int lvl){
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  
   node->type(new basic_type(4, basic_type::TYPE_POINTER));
    
}

void zu::type_checker::do_var_initialization_node(zu::var_initialization_node * const node, int lvl){
  const std::string &id = *(node->identifier()->identifier());
  std::shared_ptr<zu::symbol> symbol = _symtab.find(id);
  if(symbol != nullptr){
            if(_symtab.find_local(id) != nullptr)
                throw id + " variable has been redeclared";
            
  }

  _symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, -1));
  node->expression()->accept(this, lvl + 2);
  if(node->type()->name() != node->expression()->type()->name()){
     if((node->type()->name() == basic_type::TYPE_DOUBLE &&
        node->expression()->type()->name() == basic_type::TYPE_INT) ||
        (node->type()->name() ==basic_type::TYPE_POINTER &&
        node->expression()->type()->name() == basic_type::TYPE_INT)){
     
            return;
        }
      
      throw std::string("initialization type is not compatible with variable type");
  }

}

void zu::type_checker::do_var_declaration_node(zu::var_declaration_node * const node, int lvl){
  const std::string &id = *(node->identifier()->identifier());
    std::shared_ptr<zu::symbol> symbol = _symtab.find(id);


  if(symbol != nullptr){
            if(_symtab.find_local(id) != nullptr)
                throw id + " variable has been redeclared";
            
  }
    
  _symtab.insert(id, std::make_shared<zu::symbol>(node->type(), id, -1));
}
