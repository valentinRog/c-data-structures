#include "vds.h"
#include <stdio.h>

vds_rb_tree_set vds_rb_tree_set_create( int ( *cmp )( void *, void * ),
                                        void ( *del_key )( void * )) {
    vds_rb_tree_set tree = { 0 };
    tree.nil             = calloc( sizeof( vds_rb_tree_set_node ), 1 );
    tree.root            = tree.nil;
    tree.cmp             = cmp;
    tree.del_key         = del_key;
    return tree;
}

void vds_rb_tree_set_destroy( vds_rb_tree_set *tree ) {
    vds_rb_tree_set_clear( tree );
    free( tree->nil );
}

static void rotate_left( vds_rb_tree_set *tree, vds_rb_tree_set_node *x ) {
    vds_rb_tree_set_node *y = x->right;
    x->right                = y->left;
    if ( y->left != tree->nil ) { y->left->p = x; }
    y->p = x->p;
    if ( !x->p ) {
        tree->root = y;
    } else if ( x == x->p->left ) {
        x->p->left = y;
    } else {
        x->p->right = y;
    }
    y->left = x;
    x->p    = y;
}

static void rotate_right( vds_rb_tree_set *tree, vds_rb_tree_set_node *x ) {
    vds_rb_tree_set_node *y = x->left;
    x->left                 = y->right;
    if ( y->right != tree->nil ) { y->right->p = x; }
    y->p = x->p;
    if ( !x->p ) {
        tree->root = y;
    } else if ( x == x->p->right ) {
        x->p->right = y;
    } else {
        x->p->left = y;
    }
    y->right = x;
    x->p     = y;
}

void vds_rb_tree_set_insert( vds_rb_tree_set *tree, void *k ) {
    vds_rb_tree_set_node *p = tree->root;
    while ( p != tree->nil ) {
        if ( tree->cmp( k, p->key ) < 0 ) {
            if ( p->left == tree->nil ) { break; }
            p = p->left;
        } else if ( tree->cmp( k, p->key ) > 0 ) {
            if ( p->right == tree->nil ) { break; }
            p = p->right;
        } else {
            return;
        }
    }
    if ( p == tree->nil ) { p = NULL; }
    vds_rb_tree_set_node *node = malloc( sizeof( vds_rb_tree_set_node ) );
    node->key                  = k;
    node->p                    = p;
    node->left                 = tree->nil;
    node->right                = tree->nil;
    node->red                  = true;
    if ( !p ) {
        tree->root = node;
    } else if ( tree->cmp( k, p->key ) < 0 ) {
        p->left = node;
    } else {
        p->right = node;
    }
    while ( node != tree->root && node->p->red ) {
        if ( node->p == node->p->p->left ) {
            vds_rb_tree_set_node *y = node->p->p->right;
            if ( y->red ) {
                node->p->red    = false;
                y->red          = false;
                node->p->p->red = true;
                node            = node->p->p;
            } else {
                if ( node == node->p->right ) {
                    node = node->p;
                    rotate_left( tree, node );
                }
                node->p->red    = false;
                node->p->p->red = true;
                rotate_right( tree, node->p->p );
            }
        } else {
            vds_rb_tree_set_node *y = node->p->p->left;
            if ( y->red ) {
                node->p->red    = false;
                y->red          = false;
                node->p->p->red = true;
                node            = node->p->p;
            } else {
                if ( node == node->p->left ) {
                    node = node->p;
                    rotate_right( tree, node );
                }
                node->p->red    = false;
                node->p->p->red = true;
                rotate_left( tree, node->p->p );
            }
        }
    }
    tree->root->red = false;
    tree->size++;
}

bool vds_rb_tree_set_search( vds_rb_tree_set *tree, void *k ) {
    vds_rb_tree_set_node *p = tree->root;
    while ( p != tree->nil ) {
        if ( tree->cmp( k, p->key ) < 0 ) {
            p = p->left;
        } else if ( tree->cmp( k, p->key ) > 0 ) {
            p = p->right;
        } else {
            return true;
        }
    }
    return false;
}

static void transplant( vds_rb_tree_set      *tree,
                        vds_rb_tree_set_node *u,
                        vds_rb_tree_set_node *v ) {
    if ( !u->p ) {
        tree->root = v;
    } else if ( u == u->p->left ) {
        u->p->left = v;
    } else {
        u->p->right = v;
    }
    v->p = u->p;
}

static vds_rb_tree_set_node *tree_minimum( vds_rb_tree_set      *tree,
                                           vds_rb_tree_set_node *x ) {
    while ( x->left != tree->nil ) { x = x->left; }
    return x;
}

void vds_rb_tree_set_remove( vds_rb_tree_set *tree, void *k ) {
    vds_rb_tree_set_node *z = tree->root;
    while ( z != tree->nil ) {
        if ( tree->cmp( k, z->key ) < 0 ) {
            z = z->left;
        } else if ( tree->cmp( k, z->key ) > 0 ) {
            z = z->right;
        } else {
            break;
        }
    }
    if ( z == tree->nil ) { return; }
    vds_rb_tree_set_node *y = z;
    vds_rb_tree_set_node *x;
    bool                  y_original_color = y->red;
    if ( z->left == tree->nil ) {
        x = z->right;
        transplant( tree, z, z->right );
    } else if ( z->right == tree->nil ) {
        x = z->left;
        transplant( tree, z, z->left );
    } else {
        y                = tree_minimum( tree, z->right );
        y_original_color = y->red;
        x                = y->right;
        if ( y->p == z ) {
            x->p = y;
        } else {
            transplant( tree, y, y->right );
            y->right    = z->right;
            y->right->p = y;
        }
        transplant( tree, z, y );
        y->left    = z->left;
        y->left->p = y;
        y->red     = z->red;
    }
    if ( !y_original_color ) {
        while ( x != tree->root && !x->red ) {
            if ( x == x->p->left ) {
                vds_rb_tree_set_node *w = x->p->right;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    rotate_left( tree, x->p );
                    w = x->p->right;
                }
                if ( !w->left->red && !w->right->red ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( !w->right->red ) {
                        w->left->red = false;
                        w->red       = true;
                        rotate_right( tree, w );
                        w = x->p->right;
                    }
                    w->red        = x->p->red;
                    x->p->red     = false;
                    w->right->red = false;
                    rotate_left( tree, x->p );
                    x = tree->root;
                }
            } else {
                vds_rb_tree_set_node *w = x->p->left;
                if ( w->red ) {
                    w->red    = false;
                    x->p->red = true;
                    rotate_right( tree, x->p );
                    w = x->p->left;
                }
                if ( !w->right->red && !w->left->red ) {
                    w->red = true;
                    x      = x->p;
                } else {
                    if ( !w->left->red ) {
                        w->right->red = false;
                        w->red        = true;
                        rotate_left( tree, w );
                        w = x->p->left;
                    }
                    w->red       = x->p->red;
                    x->p->red    = false;
                    w->left->red = false;
                    rotate_right( tree, x->p );
                    x = tree->root;
                }
            }
        }
        x->red = false;
    }
    if ( tree->del_key ) { tree->del_key( z->key ); }
    free( z );
    tree->size--;
}

void vds_rb_tree_set_clear( vds_rb_tree_set *tree ) {
    while ( tree->size ) { vds_rb_tree_set_remove( tree, tree->root->key ); }
}
