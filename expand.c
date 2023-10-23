// Rimsha Rizvi
// CS 211 - PROJECT 7 - The process of “file expansion” - takes as input a file such as A and produces its ”expanded version.”

#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define TRUE  1
#define FALSE 0

typedef struct Stack{
  char * arr;  // array of chars
  char ** str_stack;  // strings in stack
  int curr;  // values currenly in the stack of strings
  int size;  // stack size
  int capacity; // maximum cap in stack
  int is_empty;  // acts like a bool to check if file is empty
} stack;


// init
// initializing the stack
stack * init(){
  stack * s = malloc(sizeof(stack));
  s->arr = malloc(256 * sizeof(char));
  s->str_stack = malloc(4 * sizeof(char*));
  s->curr = -1;
  s->size = -1;
  s->capacity = 4;
  s->is_empty = FALSE;

  for (int i = 0; i < s->capacity; i++){
     s->str_stack[i] = NULL;
  }

  return s;
}

// file_isGood
// checking if the file exists
int file_isGood(char *str){
  char *tempStr = str;
  if(access(tempStr, F_OK) == 0 ) {
    return TRUE;
  } else{
    return FALSE;
  }

}



// isCopy function is defined below and is needed for the push function
int isCopy(stack* s, char* r);

// push
// pushing/adding an element onto the stack
int push(stack * s, char * str){

  if (isCopy(s, str)){
    return 0;
  }
  s->curr++;
  s->size++;

  // grow stack if it exceeds capacity
  if (s->curr >= s->capacity){
    char** temp;
    temp = s->str_stack;

    s->str_stack = realloc(temp, s->capacity * 2 * sizeof(char*));

    s->capacity *= 2;

    for (int i = s->curr + 1; i < s->capacity; i++){
      s->str_stack[i] = NULL;
    }
  }
  s->str_stack[s->curr] = str;
  
  return 1;
}


// pop
// popping/removing an element off of the stack
void pop(stack * s){
  if (s->curr < 0){
    return;
  }
  s->curr -= 1;
}

// clear_stringNclose_file
// frees string, then closes the file
void clear_stringNclose_file(FILE * p, stack *s, char *substring){
  pop(s);
  fclose(p); 
}

// get_string
// gets the string in array
char * get_string(stack *s){
  if (!strcmp(s->arr, " ")) {
    return " ";
  }
  return s->arr;
}


// check_empty
int check_empty(stack *s){
  return s->is_empty;
}

// checkFile
// check contents of file
int checkFile(char *str){
  char *index = str;
  if (access(index, F_OK) == 0 ) {
    return TRUE;
  } else{
    return FALSE;
  }
}

// clear
// clear the stack so that it is empty and ready to be used again
void clear(stack * s){
  for (int i = 0; i <= s->size; i++){
    if (i>= s->capacity || s->str_stack[i] == NULL) {
      continue;
    }
  }
  free(s->str_stack);
  free(s->arr);
  free(s);
}


// isCopy
// check to see if a string is copied in stack
int isCopy(stack * s, char * r){
  for (int i = 0; i <= s->curr; i ++){
    if (!(strcmp(s->str_stack[i], r))){
      return TRUE;
    }
  }
  return FALSE;
}


// string_editor
// This function removes whitespaces in order to get to the first char
int string_editor(char* str){
  if (strlen(str) <= 7){
    return 0;
  }
  char* new_str = str;
  int start_index = 0;

  int right_apostrophe = FALSE;  // boolean variable to check if the right apostrophe exists
  int left_apostrophe = FALSE;  // boolean variable to check if the right apostrophe exists
  int apostrophe_exists = FALSE; // both exist?

  int count = 1;

  int i = 0;

  while (i < strlen(str)) {
    if ((str[i] != ' ') && (str[i] == '"')){
      left_apostrophe = TRUE;
      break;
    };
    count++;
    i++;
  }
  if (str[strlen(str) - 1] == '"' || str[strlen(str) - 2] == '"') {
    right_apostrophe = TRUE;
  }
  for (int i = count; i < strlen(str) - 1; i++){
    if (i != count && str[i] == '"'){
      right_apostrophe = TRUE;
      break;
    };
    new_str[start_index] = str[i];
    start_index++;
  }

  int j = start_index;

  while (j < strlen(str)) {
    new_str[j] = ' ';
    j++;
  }

  if (right_apostrophe && left_apostrophe) {
    apostrophe_exists = TRUE;
  }
  memmove(new_str, &str[0] , start_index - 1);  // Built-in function to move block of memory
  new_str[start_index] = '\0';
  str = new_str;

  return apostrophe_exists;  // 1 if edited properly, 0 if not
}


// readme

int readme(FILE * filename, stack * s){

  char input [257];  // no more than 256 characters
  
  while (fgets(input, 256, filename) != NULL){
    if (input[0] == '#' && input[7] == 'e'){
      
      char small[257];
      memcpy(small, &input[8], 256);
      small[256] = '\0';
      int app = string_editor(small);  // edits string to remove quotation marks
      if ((!file_isGood(small) || app == 0)){
        strcpy(s->arr, small);
        s->is_empty = TRUE;
        if (app == 0){
          clear_stringNclose_file(filename, s, small);  
          return -1;
        }
        if (!file_isGood(small)){
          clear_stringNclose_file(filename, s, small);
          return -2;
        }
      }
      
      //infinite loop
      int valid = push(s, small);

      FILE *file = fopen(small, "r");
      if (valid != 1) { // there's a copy
        fclose(file);
        clear_stringNclose_file(filename, s, small);
        return valid;
      };
      
      int _valid = readme(file, s);
      if (_valid != 1){
        clear_stringNclose_file(filename, s, small);
        return _valid;
      }

    } else {
      printf("%s", input);
    }
  }
  pop(s);
  fclose(filename);
  return 1;
}

int main(int argc, char *argv[]) {
    
    char * filename = "f1.txt";
    for (int i = 0; i < argc; i++){
      filename = argv[i];
    }
    
    char buff [257];
    FILE  * file = fopen(filename, "r");
    stack * s = init();
    int valid = -5;
  
    while (fgets(buff, 256, file) != NULL){
      if (buff[0] == '#' && buff[7] == 'e'){
        char substring[257];
        memcpy(substring, &buff[8], 256);
        substring[256] = '\0';
        int app = string_editor(substring);
    
        if ((!file_isGood(substring) || app == 0)){
          strcpy(s->arr, substring);
          if (app == 0){
            valid = -1;
            break;
          }
          if (!file_isGood(substring)){
            valid = -2;
            break;
          }
      }
        
        if (!strcmp(filename, substring)){
          valid = 0;
          pop(s);
          break;
        }

        FILE *file = fopen(substring, "r");
        push(s, substring);
        

        valid = readme(file, s);

        if ((valid != 1)) {
          if (!check_empty(s)){
            strcpy(s->arr, substring);
            
          }
          
          break;
        }
        
      }
      else
        printf("%s", buff);
    }
    if (valid == 0)
      fprintf(stderr, "**error: Infinite recursion detected!\nExpansion terminated.");
    else if (valid == -1)
      fprintf(stderr, "**error: No file found after '#include'!\nExpansion terminated.");
    else if (valid == -2)
      fprintf(stderr, "**error: File '%s' doesn't exist!\nExpansion terminated.", get_string(s));
    clear(s);
    fclose(file);
}
