//
// Created by Tangruiwen on 2017/2/26.
//

#ifndef MOONOS_ASSERT_H
#define MOONOS_ASSERT_H

#define ASSERT
#ifdef ASSERT
void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)  if (exp) ; \
        else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif


#endif //MOONOS_ASSERT_H
