#include <stdio.h>
#include <stdlib.h>

#include "state.h"

state_ctx *state_init(int init_state)
{
    struct state_ctx *state_ctx = calloc(1, sizeof(struct state_ctx));
    if(state_ctx == NULL)
        return state_ctx;

    state_ctx->stt_current = init_state;

    return state_ctx;
}

int state_transition_table(state_ctx *state_ctx, int current_state, int evnet, int next_state)
{
    struct fms_t * transtion = (struct fms_t *)malloc(sizeof(struct fms_t));

    if(transtion == NULL)
        return -1;

    transtion->fms_current = current_state;
    transtion->fms_evnet = evnet;
    transtion->fms_next = next_state;
    if(state_ctx->stt_transition == NULL){
        state_ctx->stt_transition = list_create(transtion);
    }else{
        state_ctx->stt_transition = list_insert_beginning(state_ctx->stt_transition,transtion);
    }

    if(state_ctx->stt_transition == NULL)
        return -1;

    return 0;
}

int state_action_map(state_ctx *state_ctx, int state, state_action action)
{
    struct action_map_t *action_map = (struct action_map_t *)malloc(sizeof(struct action_map_t));
    if(action_map == NULL)
        return -1;

    action_map->am_action = action;
    action_map->am_state = state;

    if(state_ctx->stt_actions == NULL){
        state_ctx->stt_actions = list_create(action_map);
    }else{
        state_ctx->stt_actions = list_insert_beginning(state_ctx->stt_actions,action_map);
    }

    if(state_ctx->stt_actions == NULL)
        return -1;

    return 0;
}

int state_transitiom_compare(list_node* node,const void*data)
{
    struct fms_t *fms1 = (struct fms_t *)data;
    struct fms_t *fms2 = (struct fms_t *)node->data;

    if(fms1->fms_current == fms2->fms_current &&
            fms1->fms_evnet == fms2->fms_evnet){
        return 1;
    }

    return 0;
}

int state_action_compare(list_node* node,const void*data)
{
    struct action_map_t *_map = (struct action_map_t *)node->data;
    int state = *(int *)data;

    if(_map->am_state == state)
        return 1;
    else
        return 0;
}

int state_change(state_ctx *state_ctx, int event, void *userdata)
{
    struct fms_t fms_current = {state_ctx->stt_current,event, 0 };
    struct fms_t *fms_next;
    struct list_node* node = list_find(state_ctx->stt_transition,
                                       state_transitiom_compare,&fms_current);

    if(node == NULL)  return -1;

    fms_next = (struct fms_t *)node->data;

    node = list_find(state_ctx->stt_actions,state_action_compare,&fms_next->fms_next);
    struct action_map_t *_map = (struct action_map_t *)node->data;

    if(_map == NULL)   return -1;

    _map->am_action(userdata);
}

int state_deinit(state_ctx *state_ctx)
{
    //list_foreach();
}
