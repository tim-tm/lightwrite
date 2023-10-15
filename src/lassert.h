#ifndef _LASSERT_H_
#define _LASSERT_H_

void lassert_fail(char* expr, char* file, int line);

#ifndef LASSERT
#define LASSERT(expr) if (!(expr)) lassert_fail(#expr, __FILE__, __LINE__)
#endif // !LASSERT

#endif // !_LASSERT_H_
