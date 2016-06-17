#ifndef __ZU_SYMBOL_H__
#define __ZU_SYMBOL_H__

#include <vector>
#include <cdk/basic_type.h>
#include <string>

namespace zu {

    class symbol {
      basic_type *_type;
      std::string _name;
      
      // Local vars
      int _offset = 0;      
      
      
      // Functions:
      std::vector<basic_type*> _args;
      cdk::expression_node *_return_value;
      bool _is_function_defined;   	// True if already defined 
      bool _is_function;        // False if symbol is a var.
      bool _is_global = false;
      
            
      long _value;

    public:
      inline symbol(basic_type *type, const std::string &name, long value) :
          _type(type), _name(name), _value(value) {
              _is_function = false;
      }

      inline symbol(basic_type *type, const std::string &name, std::vector<basic_type*> args, bool defined, long value) :
          _type(type), _name(name), _args(args), _is_function_defined(defined), _is_function(true), _value(value) {
      }

      virtual ~symbol() {
          delete _type;
      }

      inline basic_type *type() const {
        return _type;
      }
      inline const std::string &name() const {
        return _name;
      }
      inline long value() const {
        return _value;
      }
      inline long value(long v) {
        return _value = v;
      }
      inline std::vector<basic_type*> args() {
	return _args;
      }
      inline bool is_function_defined() {
      	return _is_function_defined;
      }
      inline void is_function_defined(bool defined){
     	 _is_function_defined = defined;
      }
      inline bool is_function() {
      	return _is_function;
      }
      inline void is_function(bool value){
     	 _is_function = value;
      }
      inline bool is_global(){
      	return _is_global;
      }
      inline void is_global(bool value){
     	 _is_global = value;
      }
      inline int offset() {
      	return _offset;
      }
      inline void offset(int off){
      	_offset = off;
      }
      inline cdk::expression_node *return_value() {
        return _return_value;
      }
      inline void return_value(cdk::expression_node* return_value) {
        _return_value = return_value;
      }
      
    };

} 

#endif
 
