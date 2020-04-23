load('api_zswitch.js');

ZenSwitch.GPIO = {
  _att: ffi('bool mgos_zswitch_gpio_attach(void *, int, void *)'),
  _det: ffi('void mgos_zswitch_gpio_detach(void *)'),
  _cfgc: ffi('void *mjs_zswitch_gpio_cfg_create(bool)'),
};

ZenSwitch._proto.GPIO = {
  _switch: null,
  _getHandle: function() {
    print('Handle', this._switch.handle, 'for', this._switch.id);
    return this._switch.handle;
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
    let cfgo = null;
    if (cfg) {
      cfgo = ZenSwitch.GPIO._cfgc(((cfg.activeHigh === undefined ||
        cfg.activeHigh === null) ? true : cfg.activeHigh));
    }
    let result = ZenSwitch.GPIO._att(this._getHandle(), pin, cfgo);
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
    ZenSwitch.GPIO._det(this._getHandle());
  },
};

ZenSwitch._proto._onCreateSub(function(obj){
  obj.GPIO._switch = obj; 
});
