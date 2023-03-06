#include "vds.h"

vds_list vds_list_create( void ( *delete )( void * ) ) {
    vds_list list = { 0 };
    list.del      = delete;
    return list;
}

void vds_list_destroy( vds_list *list ) { vds_list_clear( list ); }

void vds_list_push_back( vds_list *list, void *data ) {
    vds_list_node *node = malloc( sizeof( vds_list_node ) );
    node->data          = data;
    node->next          = NULL;
    node->prev          = list->tail;
    if ( list->tail ) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->size++;
}

void vds_list_push_front( vds_list *list, void *data ) {
    vds_list_node *node = malloc( sizeof( vds_list_node ) );
    node->data          = data;
    node->next          = list->head;
    node->prev          = NULL;
    if ( list->head ) {
        list->head->prev = node;
    } else {
        list->tail = node;
    }
    list->head = node;
    list->size++;
}

void vds_list_pop_back( vds_list *list ) {
    if ( !list->tail ) { return; }
    vds_list_node *node = list->tail;
    list->tail          = node->prev;
    if ( list->tail ) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }
    if ( list->del ) { list->del( node->data ); }
    free( node );
    list->size--;
}

void vds_list_pop_front( vds_list *list ) {
    if ( !list->head ) { return; }
    vds_list_node *node = list->head;
    list->head          = node->next;
    if ( list->head ) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }
    if ( list->del ) { list->del( node->data ); }
    free( node );
    list->size--;
}

void vds_list_clear( vds_list *list ) {
    for ( vds_list_node *node = list->head; node; ) {
        vds_list_node *next = node->next;
        if ( list->del ) { list->del( node->data ); }
        free( node );
        node = next;
    }
    bzero( list, sizeof( vds_list ) );
}
