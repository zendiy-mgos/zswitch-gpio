#include "mgos.h"
#include "mgos_gpio.h"
#include "common/queue.h"
#include "mgos_zswitch_gpio.h"


struct mg_zswitch_gpio_entry {
  struct mgos_zswitch *handle;
  int pin;
  struct mgos_zswitch_gpio_cfg cfg;
  SLIST_ENTRY(mg_zswitch_gpio_entry) entry;
};

/* Execution context */
struct mg_zswitch_gpio_ctx {
  int version;
  SLIST_HEAD(entries, mg_zswitch_gpio_entry) entries;
};

/* Excecution context instance */
static struct mg_zswitch_gpio_ctx *s_context = NULL;

bool mg_zswitch_gpio_state_cb(enum mgos_zthing_state_act act,
                              struct mgos_zswitch_state *state,
                              void *user_data) {
  struct mg_zswitch_gpio_entry *entry = (struct mg_zswitch_gpio_entry *)user_data;
  
  bool gpio_val;
  if (act == MGOS_ZTHING_STATE_SET) {
    if (state->value) {
      gpio_val = (entry->cfg.active_high ? true : false);
    } else {
      gpio_val = (entry->cfg.active_high ? false : true);
    }
    mgos_gpio_write(entry->pin, gpio_val);
    mgos_msleep(10);
    if (mgos_gpio_read(entry->pin) == gpio_val) return true;
    LOG(LL_ERROR, ("Unexpected GPIO value reading '%s' state.",
      entry->handle->id));
  } else if (act == MGOS_ZTHING_STATE_GET) {
    gpio_val = mgos_gpio_read(entry->pin);
    if (entry->cfg.active_high) {
      state->value = gpio_val ? true : false;
    } else {
      state->value = gpio_val ? false : true;
    } 
    return true;
  }
  return false;
}

struct mg_zswitch_gpio_entry *mg_zswitch_gpio_entry_get(struct mgos_zswitch *handle) {
  struct mg_zswitch_gpio_entry *e;
  SLIST_FOREACH(e, &s_context->entries, entry) {
    if (((void *)e->handle) == ((void *)handle)) return e;
  }
  return NULL;
}

void mg_zswitch_gpio_entry_reset(struct mg_zswitch_gpio_entry *entry) {
  mgos_zswitch_state_handler_reset(entry->handle);
}

bool mg_zswitch_gpio_entry_set(struct mg_zswitch_gpio_entry *entry) {
  if (mgos_zswitch_state_handler_set(entry->handle,
      mg_zswitch_gpio_state_cb, entry)) {
    if (mgos_gpio_setup_output(entry->pin, !entry->cfg.active_high)) {
      return true;
    }
    mg_zswitch_gpio_entry_reset(entry);
    LOG(LL_ERROR, ("Error initializing GPIO %d for switch '%s'.",
      entry->pin, entry->handle->id));
  } else {
    LOG(LL_ERROR, ("Error registering state handler for switch '%s'.",
      entry->handle->id));
  }
  return false;
}

bool mgos_zswitch_gpio_attach(struct mgos_zswitch *handle, int pin,
                              struct mgos_zswitch_gpio_cfg *cfg) {
  if (handle == NULL || pin < 0) return false;
  struct mg_zswitch_gpio_entry *e = calloc(1,
    sizeof(struct mg_zswitch_gpio_entry));
  if (e != NULL) {
    e->handle = handle;
    e->cfg.active_high = (cfg == NULL ? true ? cfg->active_high);
    e->pin = pin;
    
    if (mg_zswitch_gpio_entry_set(e)) {
      SLIST_INSERT_HEAD(&s_context->entries, e, entry);
    } else {
      free(e);
      e = NULL;
    }
  }
  
  return (e != NULL);
}

void mgos_zswitch_gpio_detach(struct mgos_zswitch *handle) {
  struct mg_zswitch_gpio_entry *e = mg_zswitch_gpio_entry_get(handle);
  if (e != NULL) {
    mg_zswitch_gpio_entry_reset(e);
    SLIST_REMOVE(&s_context->entries, e, mg_zswitch_gpio_entry, entry);
    free(e);
  }
}

#ifdef MGOS_HAVE_MJS

struct mgos_zswitch_gpio_cfg *mjs_zswitch_gpio_cfg_create(bool active_high) {
  struct mgos_zswitch_gpio_cfg *cfg = calloc(1,
    sizeof(struct mgos_zswitch_gpio_cfg));
  cfg->active_high = active_high;
  return cfg;
}

#endif /* MGOS_HAVE_MJS */


bool mgos_zswitch_gpio_init() {
  /* Create the context */
  s_context = calloc(1, sizeof(struct mg_zswitch_gpio_ctx));
  if (!s_context) return false;
  
  /* Initialize the context */
  s_context->version = 1;
  SLIST_INIT(&s_context->entries);
  
  return true;
}