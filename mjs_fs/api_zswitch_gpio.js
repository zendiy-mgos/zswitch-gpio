load('api_zthing.js');

ZenSwitch._proto.GPIO = {
  _att: ffi('bool mgos_zswitch_gpio_attach(void *, int, void *)'),
  _det: ffi('void mgos_zswitch_gpio_detach(void *)'),
  _cfgc: ffi('void *mjs_zswitch_gpio_cfg_create(bool)'),

  _switch: null,
  _getHandle: function() {
    this._switch.handle
  },

  // ## **`object.GPIO.attach(pin, cfg)`**
  //
  // Example:
  // ```javascript
  // if (sw.GPIO.attach(4, {activeHigh: false})) {
  //   /* success */
  // } else {
  //   /* error */
  // }
  // ```
	attach: function(pin, cfg) {
    if (!cfg) cfg = {};
    let cfgo = this._cfgc(cfg.activeHigh || true);
    let result = this._det(this._getHandle(), pin, cfgo);
    ZenThing._free(cfgo);
    return result;
  },

  // ## **`object.GPIO.detach()`**
  //
  // Example:
  // ```javascript
  // sw.GPIO.detach();
  // ```
  detach: function() {
    this._det(this._getHandle());
  },
};

ZenSwitch._proto._onCreateSub(function(obj){
	obj.GPIO._switch = obj; 
});
