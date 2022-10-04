/*
 * Word count application with one thread per input file.
 *
 * You may modify this file in any way you like, and are expected to modify it.
 * Your solution must read each input file from a separate thread. We encourage
 * you to make as few changes as necessary.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>

#include "word_count.h"
#include "word_helpers.h"

/*
 * main - handle command line, spawning one thread per file.
 */
typedef struct threaded_arg {
  word_count_list_t* wclist;
  char* file;
} threaded_arg_t;

void* threads_count_words(void* t)
{
  threaded_arg_t* mm = (threaded_arg_t*) t;
  //char** arg = (char**) t;
  //char* file = arg[1];
  //printf("%s\n", file);
  FILE *fp = fopen(mm->file, "r");
  count_words(mm->wclist, fp);
  fclose(fp);
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  int rc;
  long t;
  /* Create the empty data structure. */
  word_count_list_t word_counts;
  init_words(&word_counts);
  word_count_list_t* ptr = &word_counts;
  if (argc <= 1) {                                          //argc==2呢？？？？？
    /* Process stdin in a single thread. */
    count_words(ptr, stdin);
  } else {
    /* TODO */
    int nthreads = argc - 1;
    pthread_t threads[nthreads];
    //char* cptr = (char*) ptr;
    for (t = 0; t < nthreads; t++) {
      char* FileName = argv[t+1];
      threaded_arg_t* mm = malloc(sizeof(threaded_arg_t));
      mm->wclist = ptr;
      mm->file = FileName;
      rc = pthread_create(&threads[t], NULL, threads_count_words, (void*)mm);
      if (rc) {
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
      }
    }
    for (long i = 0; i < nthreads; i++) {
      pthread_join(threads[i], 0);
    }
  }

  /* Output final result of all threads' work. */
  wordcount_sort(&word_counts, less_count);
  fprint_words(&word_counts, stdout);
  pthread_mutex_destroy(&(word_counts.lock));
  return 0;
}
