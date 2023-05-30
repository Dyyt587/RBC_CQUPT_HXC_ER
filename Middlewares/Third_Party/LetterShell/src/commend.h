/*
 * commend.h
 *
 *  Created on: 2023Äê3ÔÂ2ÈÕ
 *      Author: Administrator
 */

#ifndef CODE_DRIVER_COMMEND_H_
#define CODE_DRIVER_COMMEND_H_

typedef enum {
     _8=0U,

     _16,

     _32,
      _64,

     _f,
     _lf
}var_type_e;
typedef struct _var_list {
    const char* name;
    void* body;
    var_type_e type;
    struct _var_list *next;
}var_list_t;
#define VAR_CMD_REGISTER(var,type) do{\
    var_register(&var,#var,type);\
}while (0)
#define VAR_CMD_ARR_REGISTER(var,type,size) do{\
    var_arr_register(&var,#var,type,size);\
}while (0)
void var_register(void* var,const char* name, var_type_e type);
void var_arr_register(void *var,const char* name, var_type_e type,int size);
void var_init(void);


#endif /* CODE_DRIVER_COMMEND_H_ */
