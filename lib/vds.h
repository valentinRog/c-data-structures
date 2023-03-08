#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------- list ---------------------------------- */

typedef struct vds_list_node {
    void                 *data;
    struct vds_list_node *next;
    struct vds_list_node *prev;
} vds_list_node;

typedef struct vds_list {
    vds_list_node *head;
    vds_list_node *tail;
    size_t         size;
    void ( *del )( void * );
} vds_list;

vds_list vds_list_create( void ( *delete )( void * ) );
void     vds_list_destroy( vds_list *list );
void     vds_list_push_back( vds_list *list, void *data );
void     vds_list_push_front( vds_list *list, void *data );
void     vds_list_pop_back( vds_list *list );
void     vds_list_pop_front( vds_list *list );
void     vds_list_clear( vds_list *list );

/* ------------------------------- rb_tree_map ------------------------------ */

typedef struct vds_rb_tree_map_node {
    void                        *key;
    void                        *value;
    struct vds_rb_tree_map_node *p;
    struct vds_rb_tree_map_node *left;
    struct vds_rb_tree_map_node *right;
    bool                         red;
} vds_rb_tree_map_node;

typedef struct vds_rb_tree_map {
    vds_rb_tree_map_node *root;
    vds_rb_tree_map_node *nil;
    int ( *cmp )( void *, void * );
    void ( *del_key )( void * );
    void ( *del_value )( void * );
    size_t size;
} vds_rb_tree_map;

vds_rb_tree_map vds_rb_tree_map_create( int ( *cmp )( void *, void * ),
                                        void ( *del_key )( void * ),
                                        void ( *del_value )( void * ) );
void            vds_rb_tree_map_destroy( vds_rb_tree_map *tree );
void  vds_rb_tree_map_insert( vds_rb_tree_map *tree, void *k, void *v );
void *vds_rb_tree_map_find( vds_rb_tree_map *tree, void *k );
bool  vds_rb_tree_map_search( vds_rb_tree_map *tree, void *k );
void  vds_rb_tree_map_remove( vds_rb_tree_map *tree, void *k );
void  vds_rb_tree_map_clear( vds_rb_tree_map *tree );

/* ------------------------------- rb_tree_set ------------------------------ */

typedef struct vds_rb_tree_set_node {
    void                        *key;
    struct vds_rb_tree_set_node *p;
    struct vds_rb_tree_set_node *left;
    struct vds_rb_tree_set_node *right;
    bool                         red;
} vds_rb_tree_set_node;

typedef struct vds_rb_tree_set {
    vds_rb_tree_set_node *root;
    vds_rb_tree_set_node *nil;
    int ( *cmp )( void *, void * );
    void ( *del_key )( void * );
    size_t size;
} vds_rb_tree_set;

vds_rb_tree_set vds_rb_tree_set_create( int ( *cmp )( void *, void * ),
                                        void ( *del_key )( void * ) );
void            vds_rb_tree_set_destroy( vds_rb_tree_set *tree );
void            vds_rb_tree_set_insert( vds_rb_tree_set *tree, void *k );
bool            vds_rb_tree_set_search( vds_rb_tree_set *tree, void *k );
void            vds_rb_tree_set_remove( vds_rb_tree_set *tree, void *k );
void            vds_rb_tree_set_clear( vds_rb_tree_set *tree );

/* ---------------------------------- array --------------------------------- */

typedef struct vds_array {
    void  *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} vds_array;

vds_array vds_array_create( size_t element_size );
void      vds_array_destroy( vds_array *array );
void      vds_array_push( vds_array *array, void *data );

/* -------------------------------------------------------------------------- */
