/*
 * Implementation of the word_count interface using Pintos lists and pthreads.
 *
 * You may modify this file, and are expected to modify it.
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

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_lp.c"
#endif

#ifndef PTHREADS
#error "PTHREADS must be #define'd when compiling word_count_lp.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t* wclist) { 
  /* TODO */
  list_init(&wclist->lst);
  pthread_mutex_init(&wclist->lock, 0);
  return ;
}


size_t len_words(word_count_list_t* wclist) {
  /* TODO */
  pthread_mutex_lock(&wclist->lock);
  size_t cd = 0;
  struct list_elem *ls;
  for (ls = list_begin(&wclist->lst); ls != list_end(&wclist->lst); ls = list_next(ls)) 
  {
    cd++;
  }
  pthread_mutex_unlock(&wclist->lock);
  return cd;
}


word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  pthread_mutex_lock(&wclist->lock);
  struct list_elem *ls;
  for (ls = list_begin(&wclist->lst); ls != list_end(&wclist->lst); ls = list_next(ls)) 
  {
    word_count_t *countt = list_entry(ls, word_count_t, elem);
    if (!strcmp(word, countt->word)) {
      pthread_mutex_unlock(&wclist->lock);
      return countt;
    }
  }
  pthread_mutex_unlock(&wclist->lock);
  return NULL;
}

static bool li_ls(const struct list_elem *ewc1, const struct list_elem *ewc2, void *aux) {
  word_count_t *counttx = list_entry(ewc1, word_count_t, elem);
  word_count_t *countty = list_entry(ewc2, word_count_t, elem);
  bool (*aux_hs)(const word_count_t *, const word_count_t *) = aux;
  return aux_hs(counttx, countty);
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem *ls;
  pthread_mutex_lock(&wclist->lock);
  for (ls = list_begin(&wclist->lst); ls != list_end(&wclist->lst); ls = list_next(ls)) 
  {
    word_count_t *countt = list_entry(ls, word_count_t, elem);
    if (!strcmp(word, countt->word)) {
      countt->count++;
      pthread_mutex_unlock(&wclist->lock);
      return countt;
    }
  }
  word_count_t *countt = malloc(sizeof(word_count_t));
  countt->count = 1;
  countt->word = malloc(64 * sizeof(char));
  strcpy(countt->word, word);
  list_push_back(&wclist->lst, &countt->elem);
  pthread_mutex_unlock(&wclist->lock);
  return countt;
}


void fprint_words(word_count_list_t* wclist, FILE* outfile) { 
  /* TODO */
  struct list_elem *ls;
  pthread_mutex_lock(&wclist->lock);
  for (ls = list_begin(&wclist->lst); ls != list_end(&wclist->lst); ls = list_next(ls)) 
  {
    word_count_t *countt = list_entry(ls, word_count_t, elem);
    fprintf(outfile, "%d\t%s\n", countt->count, countt->word);
  }
  pthread_mutex_unlock(&wclist->lock);
}

void wordcount_sort(word_count_list_t* wclist, bool less(const word_count_t*, const word_count_t*)) {
  /* TODO */
    pthread_mutex_lock(&wclist->lock);
    list_sort(wclist, li_ls, less);
    pthread_mutex_unlock(&wclist->lock);
}
