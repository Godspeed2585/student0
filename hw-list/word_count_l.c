/*
 * Implementation of the word_count interface using Pintos lists.
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
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif

#include "word_count.h"

void init_words(word_count_list_t* wclist) { 
  /* TODO */
  list_init(wclist);
  return;
}

size_t len_words(word_count_list_t* wclist) {
  // /* TODO */
  // // size_t len = 0;
  // // struct list_elem *e;
  // // for(e = list_begin(wclist); e != list_end (&foo_list); e = list_next (e)))
  // // {
  // //   len++;
  // // }
  return list_size(wclist);
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  struct list_elem *e;
  for(e = list_begin(wclist); e != list_end (wclist); e = list_next (e))
  {
    word_count_t* big_elem = list_entry (e, word_count_t, elem);
    if(!strcmp(word, big_elem->word))
    {
      return big_elem;
    }
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  /* TODO */
  word_count_t* t_BigElem = find_word(wclist, word);
  if(t_BigElem == NULL)
  {
    word_count_t* ptr_big_element = malloc(sizeof(word_count_t));
    ptr_big_element->word = (char*) malloc(32 * sizeof(char));
    
    if(!ptr_big_element)
    {
      return NULL;
    }
    ptr_big_element->count = 1;
    strcpy(ptr_big_element->word, word);
    list_insert(list_tail(wclist), &(ptr_big_element->elem));
    return ptr_big_element;
  }
  else
  {
    t_BigElem->count++;
    return t_BigElem;
  }
  return NULL;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { 
  /* TODO */
  struct list_elem *e;
  for(e = list_begin(wclist); e != list_end (wclist); e = list_next (e))
  {
    word_count_t* big_elem = list_entry (e, word_count_t, elem);
    fprintf(outfile, "%s: %d\n", big_elem->word, big_elem->count);
  }
  return;
}
static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* TODO */
  word_count_t* big_elem1 = list_entry (ewc1, word_count_t, elem);
  word_count_t* big_elem2 = list_entry (ewc2, word_count_t, elem);
  bool (*aux_fun) (const word_count_t *, const word_count_t *) = aux;
  return aux_fun(big_elem1, big_elem2);
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  list_sort(wclist, less_list, less);
  
}
