#include "utils.h"

char* get_sub_string(char* haystack, char* needle)
{
  // Return NULL if either of them is NULL
  if (haystack == NULL || needle == NULL)
    return NULL;

  return strstr(haystack, needle);
}

char* get_token(char* string, char* delim, int offset)
{
  // Will only parse two token strings offset = 0 gives first string and offset = 1 give second string
  char* ptr = get_sub_string_after(string, delim);
  int len;

  if (ptr == NULL) {
    if (strlen(string) != 0 && offset == 0) // Case when string is sent without second token mentioned
      return strdup(string);
    else
      return NULL;
  }
  if (offset == 1)
    return strdup(ptr);

  ptr += offset;
  len = ptr - string - 1;
  return strndup(string, len);
}
char* get_sub_string_after(char* haystack, char* needle)
{
  char* ptr;
  if (haystack == NULL || needle == NULL)
    return NULL;
  ptr = strstr(haystack, needle);
  ptr += (ptr) ? strlen(needle) : 0; // Pointer now points to string after needle has compeletely occurred
  return ptr;
}

void extract_file_name_and_folder(char* buffer, file_attr_struct* file_attr, int flag)
{
  char* ptr;
  int temp_len;
  DEBUGSS("Buffer to extract from", buffer);
  ptr = get_file_name_pointer_from_path(buffer);

  if (flag == EXTRACT_LOCAL) {

    if (ptr) {
      strcpy(file_attr->local_file_name, ptr);
      temp_len = ptr - buffer;
      strncpy(file_attr->local_file_folder, buffer, temp_len);
      DEBUGSS("Local file name", file_attr->local_file_name);
      DEBUGSS("Local file folder", file_attr->local_file_folder);
    } else {
      if ((ptr = get_sub_string(buffer, ROOT_FOLDER_STR)) != 0) {
        strcpy(file_attr->local_file_folder, buffer);
        DEBUGSS("Local file folder", file_attr->local_file_folder);
      } else {
        strcpy(file_attr->local_file_name, buffer);
        DEBUGSS("Local file name", file_attr->local_file_name);
      }
    }

  } else if (flag == EXTRACT_REMOTE) {

    if (ptr) {
      strcpy(file_attr->remote_file_name, ptr);
      temp_len = ptr - buffer;
      strncpy(file_attr->remote_file_folder, buffer, temp_len);
      DEBUGSS("Remote file name", file_attr->remote_file_name);
      DEBUGSS("Remote file folder", file_attr->remote_file_folder);
    } else {
      if ((ptr = get_sub_string(buffer, ROOT_FOLDER_STR)) != 0) {
        strcpy(file_attr->remote_file_folder, buffer);
        DEBUGSS("Remote file folder", file_attr->remote_file_folder);
      } else {
        strcpy(file_attr->remote_file_name, buffer);
        DEBUGSS("Remote file name", file_attr->remote_file_name);
      }
    }
  }
}

int get_count_str_chr(char* buffer, char chr)
{
  int count = 0, i;
  if (buffer == NULL)
    return 0;

  for (i = 0; i < strlen(buffer); i++) {
    if (buffer[i] == chr)
      count++;
  }
  return count;
}
char* get_file_name_pointer_from_path(char* buffer)
{
  char* ptr;
  int buffer_len;
  if (buffer == NULL)
    return NULL;
  buffer_len = strlen(buffer);
  ptr = strrchr(buffer, ROOT_FOLDER_CHR);
  if (ptr == NULL || ptr - buffer + 1 == buffer_len) // Last Character is "/"
    return NULL;
  return ++ptr;
}

bool compare_user_struct(user_struct* u1, user_struct* u2)
{
  return (strcmp(u1->username, u2->username) == 0 && strcmp(u1->password, u2->password) == 0) ? true : false;
}
bool check_user_struct(user_struct** user)
{
  if (*user == NULL) {
    *user = (user_struct*)malloc(sizeof(user_struct));
    return false;
  }
  return true;
}

void free_user_struct(user_struct* user)
{
  free(user->username);
  free(user->password);
}

int get_md5_sum_hash_mod(char* file_path)
{
  u_char c[MD5_DIGEST_LENGTH], data[MAXFILEBUFF];
  FILE* in_file = fopen(file_path, "rb");

  if (in_file <= 0) {
    fprintf(stderr, "Unable to open file: %s", file_path);
    return -1;
  }
  MD5_CTX md_context;
  int bytes, i, mod;

  MD5_Init(&md_context);
  memset(data, 0, sizeof(data));
  while ((bytes = fread(data, 1, MAXFILEBUFF, in_file)) != 0)
    MD5_Update(&md_context, data, bytes);
  MD5_Final(c, &md_context);
  fprintf(stderr, "DEBUG: md5sum value of file ");

  for (i = 0, mod = 0; i < MD5_DIGEST_LENGTH; i++) {
    fprintf(stderr, "%02x", c[i]);
    mod = (mod * 16 + (u_int)c[i]) % NUM_SERVER;
  }
  fprintf(stderr, " Mod: %d", mod);
  fprintf(stderr, "\n");

  return mod;
}

void free_file_split_struct(file_split_struct* file_split)
{
  free(file_split->file_name);
  while (--file_split->split_count) {
    free_split_struct(file_split->splits[file_split->split_count]);
  }
}

void free_split_struct(split_struct* split)
{
  free(split->content);
}

void print_file_split_struct(file_split_struct* file_split)
{
  int i = 0;
  DEBUGS("Printing File Split Struct");
  DEBUGSS("Filename", file_split->file_name);
  DEBUGSN("Number of splits", file_split->split_count);
  for (i = 0; i < file_split->split_count; i++) {
    print_split_struct(file_split->splits[i]);
    DEBUGS("");
  }
}

void print_split_struct(split_struct* split)
{
  DEBUGSN("Split with id", split->id);
  DEBUGSN("Content_length", split->content_length);
  DEBUGSS("Content", (char*)split->content);
}
