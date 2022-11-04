

/*
**
**  [func] - _itoa.
**  [desc] -
**  [entr] - int n; the integer value to convert.
**           char *buf; the pointer to the destination memory buffer.
**           int radix; the conversion number base.
**  [exit] - char *; buf.
**  [prec] - buf is a valid memory pointer.
**  [post] - the memory pointed to by buf is modified.
**
*/
char *_itoa(int n, char *buf, int radix);
