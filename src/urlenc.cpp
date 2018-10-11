#include "urlenc.h"


int urlenc::decoded_length(const char* str)
{
  int i, n;
  for (i = n = 0; str[i] != '\0'; i++)
    if (str[i] == '%') n++;
  if (str[i-1] == '%' || str[i-2] == '%') n--; // ignore % at the end
  return i - 2*n;
}

int urlenc::decode(const char * encoded, char* buff, unsigned int len)
{
  int i,j;
  for (i = j = 0; encoded[i] != '\0' && j < len-1; i++) {
    if (encoded[i] == '%' && encoded[i+1] != '\0' && encoded[i+2] != '\0') {
      buff[j++] = urlenc::enctochar(encoded+i);
      i += 2;
    } else {
      buff[j++] = encoded[i];
    }
  }
  buff[j++] = '\0';
  return j;
}

char urlenc::enctochar(const char * encoded)
{
  if (encoded[0] != '%') return encoded[0]; // stupid check
  if (encoded[1] == '\0' || encoded[2] == '\0') return '\0'; // hacker check
  return  16 * hexval(encoded[1]) + hexval(encoded[2]);
}

char urlenc::hexval(char val)
{
  val -= 48;
  if (val > 9) val -= 7;
  if (val > 15) val -= 32;
  return val % 16;
}

int urlenc::query_key_pos(const char * query, const char * key)
{
  int i = 0, j = 0;
  char q, k;

  if (key[0] == '\0') return -1;

  do {
    q = query[i+j];
    k = key[j];
    if (k == '\0' && q == '=')
      return i;

    if (q == k) {
      j++;
    } else i++;

  } while (q != '\0');
  return -1;
}
