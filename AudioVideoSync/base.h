#ifndef _BASE_H
#define _BASE_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool if_handle_true(void* handle, const char* m, int line);

#define if_handle(a) if_handle_true((a),__FUNCTION__,__LINE__)





#endif // !_BASE_H

