#include "utils.h"
#include <stdio.h>
#ifndef NETUTILS_H
#define NETUTILS_H

#define AUTH_TEMPLATE "FLAG %d USERNAME %s PASSWORD %s"
#define AUTH_OK "AUTH_OK"
#define AUTH_NOT_OK "AUTH_NOT_OK"

enum CommonConstants {
  LIST_FLAG = 0,
  GET_FLAG = 1,
  PUT_FLAG = 2,
  MKDIR_FLAG = 3,
  AUTH_FLAG = 4,
  MAX_SEG_SIZE = 512
};

int encode_user_struct(char*, user_struct*);
void decode_user_struct(char*, user_struct*);
#endif
