/*
    Copyright (C) 2009-2016 Red Hat

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _SSS_CRYPTO_H_
#define _SSS_CRYPTO_H_

#include <talloc.h>
#include <stdint.h>

/* Does its best to generate crypto strong random int but fallbacks to
 * plain `rand()` in case of failure.
 * Thus *not* suitable to be used in security relevant context.
 */
int sss_rand(void);

/* Guaranteed either to fill given buffer with crypto strong random data
 * or to fail with error code (for example in the case of the lack of
 * proper entropy)
 * Suitable to be used in security relevant context.
 */
int sss_generate_csprng_buffer(uint8_t *buf, size_t size);

int s3crypt_sha512(TALLOC_CTX *mmectx,
                   const char *key, const char *salt, char **_hash);
int s3crypt_gen_salt(TALLOC_CTX *memctx, char **_salt);

/* Methods of obfuscation. */
enum obfmethod {
    AES_256,
    NUM_OBFMETHODS
};

char *sss_base64_encode(TALLOC_CTX *mem_ctx,
                        const unsigned char *in,
                        size_t insize);

unsigned char *sss_base64_decode(TALLOC_CTX *mem_ctx,
                                 const char *in,
                                 size_t *outsize);

#define SSS_SHA1_LENGTH 20

int sss_hmac_sha1(const unsigned char *key,
                  size_t key_len,
                  const unsigned char *in,
                  size_t in_len,
                  unsigned char *out);

int sss_password_encrypt(TALLOC_CTX *mem_ctx, const char *password, int plen,
                         enum obfmethod meth, char **obfpwd);

int sss_password_decrypt(TALLOC_CTX *mem_ctx, char *b64encoded,
                         char **password);

enum encmethod {
    AES256CBC_HMAC_SHA256,
    NUM_ENCMETHODS
};

int sss_encrypt(TALLOC_CTX *mem_ctx, enum encmethod enctype,
                const uint8_t *key, size_t keylen,
                const uint8_t *plaintext, size_t plainlen,
                uint8_t **ciphertext, size_t *cipherlen);
int sss_decrypt(TALLOC_CTX *mem_ctx, enum encmethod enctype,
                const uint8_t *key, size_t keylen,
                const uint8_t *ciphertext, size_t cipherlen,
                uint8_t **plaintext, size_t *plainlen);

#endif /* _SSS_CRYPTO_H_ */
