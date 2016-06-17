#include <string>
#include "targets/declarations_counter.h"
#include "ast/all.h"  // all.h is automatically generated 

        



void zu::declarations_counter::do_function_definition_node(zu::function_definition_node * const node, int lvl){
	if(node->return_type() != nullptr){
                    _size += node->return_type()->size();
             if(node->return_type()->name() != basic_type::TYPE_DOUBLE)
                     _size += 4;
             else 
                     _size += 8;
        }
	node->body()->accept(this, lvl + 2);
}


void zu::declarations_counter::do_block_node(zu::block_node * const node, int lvl){
	if(node->declarations()!=NULL)
		node->declarations()->accept(this, lvl+1);
	if(node->instructions()!=NULL)
		node->instructions()->accept(this, lvl+1);
}

void zu::declarations_counter::do_var_declaration_node(zu::var_declaration_node * const node, int lvl){
	_size += node->type()->size();
}

void zu::declarations_counter::do_var_initialization_node(zu::var_initialization_node * const node, int lvl){
	_size += node->type()->size();
}


void zu::declarations_counter::do_sequence_node(cdk::sequence_node * const node, int lvl){
	for (size_t i = 0; i < node->size(); i++) {
            node->node(i)->accept(this, lvl);
        }
}

void zu::declarations_counter::do_for_node(zu::for_node * const node, int lvl){
    // the only places where we can have var declrs
    if(node->init() != nullptr)
            node->init()->accept(this, lvl);
    if(node->block() != nullptr)
            node->block()->accept(this, lvl);
}
