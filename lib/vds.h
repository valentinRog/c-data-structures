#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------- list ---------------------------------- */

typedef struct vds_list_node {
    void *                data;
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

/* --------------------------------- rb_tree -------------------------------- */

typedef struct vds_rb_tree_node {
    void *                   key;
    void *                   value;
    struct vds_rb_tree_node *p;
    struct vds_rb_tree_node *left;
    struct vds_rb_tree_node *right;
    bool                     red;
} vds_rb_tree_node;

typedef struct vds_rb_tree {
    vds_rb_tree_node *root;
    vds_rb_tree_node *nil;
    int ( *cmp )( void *, void * );
    void ( *del_key )( void * );
    void ( *del_value )( void * );
    size_t size;
} vds_rb_tree;

vds_rb_tree vds_rb_tree_create( int ( *cmp )( void *, void * ),
                                void ( *del_key )( void * ),
                                void ( *del_value )( void * ) );
void        vds_rb_tree_destroy( vds_rb_tree *tree );
void        vds_rb_tree_insert( vds_rb_tree *tree, void *k, void *v );
void *      vds_rb_tree_search( vds_rb_tree *tree, void *k );
void        vds_rb_tree_remove( vds_rb_tree *tree, void *k );
void        vds_rb_tree_clear( vds_rb_tree *tree );

/* -------------------------------------------------------------------------- */
