namespace urlenc
{
  int decoded_length(const char*);
  int decode(const char *, char*, unsigned int);

  char enctochar(const char *);
  char hexval(char);

  int query_key_pos(const char *, const char *);
}
