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
char *_itoa(int n, char *buf, int radix)
{
  char         *ret = buf;
  char         tmp[33];
  int          i = 0, j, r;

  /* validate the conversion number base. */
  if ((radix >= 2) && (radix <= 36)) {
    if ((radix == 10) && (n < 0)) {
      /* negative integer value. */
      *buf++ = '-';
      n = -n;
    }
    do {
      /* calculate the current digit. */
      r = (int)((unsigned int)n % radix);
      tmp[i++] = ((r < 10) ? (r + '0') : (r - 10 + 'a'));
    } while ((n /= radix) != 0);
    /* reverse the buffer string. */
    for (--i, j = 0; (i >= 0); --i, ++j) buf[j] = tmp[i];
    buf[j] = 0;
  }
  return (ret);
}
