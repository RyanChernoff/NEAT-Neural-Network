#include <stdlib.h>
#include <stdbool.h>

typedef void *entry;
typedef void *key;

typedef struct dict_node dict_node;
struct dict_node {
  key key;
  entry entry;
  dict_node *next;
};

typedef unsigned int key_hash_fn(key k);
typedef bool key_equiv_fn(key k1, key k2);
typedef void key_free_fn(key k);
typedef void entry_free_fn(entry k);
typedef key key_copy_fn(key k);
typedef entry entry_copy_fn(entry k);

struct dict_header{
  size_t size;
  size_t compacity;
  dict_node **buckets;
  key_hash_fn *hash;
  key_equiv_fn *equiv;
  key_free_fn *key_free;
  entry_free_fn *entry_free;
};
typedef struct dict_header dict;

dict *dict_new(size_t compacity, key_hash_fn *hash, key_equiv_fn *equiv, key_free_fn key_free, entry_free_fn *entry_free){
  dict *D = malloc(sizeof(dict));
  D->size = 0;
  D->compacity = compacity;
  D->buckets = calloc(compacity, sizeof(dict_node *));
  D->hash = hash;
  D->equiv = equiv;
  D->key_free = key_free;
  D->entry_free = entry_free;
  return D;
}

unsigned int dict_hash(dict *D, key k){
  return (*D->hash)(k) % D->compacity;
}

dict_node *find_node(dict *D, key k){
  unsigned int hash = dict_hash(D, k);
  dict_node *node = D->buckets[hash];
  while(node != NULL){
    if((*D->equiv)(node->key, k)) return node;
    node = node->next;
  }
  return NULL;
}

void dict_resize(dict *D){
  size_t old_compacity = D->compacity;
  D->compacity *= 2;
  dict_node **new_buckets = calloc(D->compacity, sizeof(dict_node *));
  for(size_t i = 0; i < old_compacity; i++){
    dict_node *node = D->buckets[i];
    while(node != NULL){
      dict_node *next = node->next;
      unsigned int hash = dict_hash(D, node->key);
      node->next = new_buckets[hash];
      new_buckets[hash] = node;
      node = next;
    }
  }
  free(D->buckets);
  D->buckets = new_buckets;
}

void dict_add(dict *D, key k, entry e){
  dict_node *node = find_node(D, k);
  if(node != NULL){
    if(D->entry_free != NULL) (*D->entry_free)(node->entry);
    node->entry = e;
    return;
  }
  node = malloc(sizeof(dict_node));
  unsigned int hash = dict_hash(D, k);
  node->key = k;
  node->entry = e;
  node->next = D->buckets[hash];
  D->buckets[hash] = node;
  D->size++;
  if(D->size >= D->compacity * 1.5) dict_resize(D);
}

entry dict_get(dict *D, key k){
  dict_node *node = find_node(D, k);
  if(node == NULL) return NULL;
  return node->entry;
}

entry dict_remove(dict *D, key k){
  unsigned int hash = dict_hash(D, k);
  dict_node *node = D->buckets[hash];
  dict_node *prev = NULL;
  while(node != NULL){
    if((*D->equiv)(node->key, k)){
      if(prev == NULL) D->buckets[hash] = node->next;
      else prev->next = node->next;
      if(D->key_free != NULL) (*D->key_free)(node->key);
      entry e = node->entry;
      free(node);
      D->size--;
      return e;
    }
  }
  return NULL;
}

dict *dict_copy(dict *D, key_copy_fn *key_copy, entry_copy_fn *entry_copy){
  dict *C = dict_new(D->compacity, D->hash, D->equiv, D->key_free, D->entry_free);
  for(size_t i = 0; i < D->compacity; i++){
    dict_node *node = D->buckets[i];
    while(node != NULL){
      dict_node *new_node = malloc(sizeof(dict_node));
      new_node->key = (*key_copy)(node->key);
      new_node->entry = (*entry_copy)(node->entry);
      new_node->next = C->buckets[i];
      C->buckets[i] = new_node;
      node = node->next;
    }
  }
  C->size = D->size;
  return C;
}

void dict_free(dict *D){
  for(size_t i = 0; i < D->compacity; i++){
    dict_node *node = D->buckets[i];
    while(node != NULL){
      dict_node *next = node->next;
      if(D->key_free != NULL) (*D->key_free)(node->key);
      if(D->entry_free != NULL) (*D->entry_free)(node->entry);
      free(node);
      node = next;
    }
  }
  free(D->buckets);
  free(D);
}