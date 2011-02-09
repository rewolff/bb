

enum bb_types {BB_INVALID = -1,
	       BB_BYTE, BB_SHORT, BB_INT, BB_FLOAT, BB_BIT};

enum bb_errno {BB_OK = 0,
	       BB_ENOTFOUND};  // Expand later. 


enum bb_types bb_typestring_to_enum (char *s);
char * bb_type_enum_to_string (enum bb_types t);

int bb_typesize (enum bb_types t);

void bb_init ();
int bb_get_type (char *name);
void *bb_get_ptr (char *name);
void bb_create_var (char *name, int type);
enum bb_errno bb_overlay_var (char *name1, int type, char *where, int offset);
int bb_getbitval (void *p);
void bb_setbit (void *p);
void bb_clrbit (void *p);
void bb_writebit (void *p, int val);

