#ifndef BB_LIB_H
#define BB_LIB_H


enum bb_types {BB_INVALID = -1,
	       BB_BYTE, BB_SHORT, BB_INT, BB_FLOAT, BB_BIT};

enum bb_errno {BB_OK = 0,
	       BB_ENOTFOUND};  // Expand later. 

// Struct bb_var should be opague: It is not defined HERE!. 
// struct bb_var; 

enum bb_types bb_typestring_to_enum (char *s);
char * bb_type_enum_to_string (enum bb_types t);

int bb_typesize (enum bb_types t);

void bb_init ();
int bb_get_type (char *name);
void *bb_get_ptr (char *name);
struct bb_var *bb_get_handle (char *name);
void bb_create_var (char *name, int type);
enum bb_errno bb_overlay_var (char *name1, int type, char *where, int offset);
int bb_getbitval (void *p);
void bb_set_bit (struct bb_var *p);
void bb_clr_bit (struct bb_var *p);
void bb_write_bit (struct bb_var *p, int val);
void bb_write_int (struct bb_var *p, int val);
void bb_write_float (struct bb_var *p, float val);

int   bb_get_int (struct bb_var *p);
float bb_get_float (struct bb_var *p);

struct bb_var *bb_get_next_var(struct bb_var *p);
char *bb_get_name(struct bb_var *p);

char *bb_get_base (void);

#endif
