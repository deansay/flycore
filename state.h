#ifndef __STATE_T_H__
#define __STATE_T_H__
#include "list.h"

typedef void (*state_action)(void *userdata);

typedef struct fms_t {
    int fms_current;
    int fms_evnet;
    int fms_next;
}fms_t;

typedef struct action_map_t{
    int          am_state;
    state_action am_action;
}action_map_t;

typedef struct state_ctx {
    list_node *stt_transition;
    list_node *stt_actions;
    int       stt_current;
}state_ctx;

struct state_ctx *state_init(int init_state);
int state_transition_table(struct state_ctx *state_ctx,int current_state, int evnet,int next_state);
int state_action_map(struct state_ctx *state_ctx,int state, state_action action);
int state_change(struct state_ctx *state_ctx,int event, void *userdata);
int state_deinit(struct state_ctx *state_ctx);

#endif /*__STATE_T_H__*/
