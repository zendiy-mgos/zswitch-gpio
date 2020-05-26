# ZenSwitch GPIO
## Overview
Mongoose OS library for GPIO-enabled ZenSwitches.
## GET STARTED
Build up your own device in few minutes just starting from one of the following samples.

|Sample|Notes|
|--|--|
|[zswitch-gpio-demo](https://github.com/zendiy-mgos/zswitch-gpio-demo)|Mongoose OS demo firmware for using GPIO-enabled ZenSwitches.|
## Usage
Include the library into your `mos.yml` file.
```yaml
libs:
  - origin: https://github.com/zendiy-mgos/zswitch-gpio
```
If you are developing a JavaScript firmware, load `api_zswitch_gpio.js` in your `init.js` file.
```js
load('api_zswitch_gpio.js');
```
## C/C++ API Reference
### mgos_zswitch_gpio_cfg
```c
struct mgos_zswitch_gpio_cfg {
  bool active_high;
};
```
GPIO configuration settings for `mgos_zswitch_gpio_attach()`.

|Field||
|--|--|
|active_high|`true` if switch is ON when GPIO output is high (1)|
### mgos_zswitch_gpio_attach()
```c
bool mgos_zswitch_gpio_attach(struct mgos_zswitch *handle, int pin, struct mgos_zswitch_gpio_cfg *cfg);
```
Attach the switch to the GPIO. Returns `true` on success, `false` otherwise.

|Parameter||
|--|--|
|handle|Switch handle.|
|pin|GPIO pin.|
|cfg|Optional. GPIO configuration. If `NULL`, following default configuration values are used: active_high=`true`.|

**Example** - Create a switch using default configuration values and attach it to the GPIO 5.
```c
struct mgos_zswitch *sw = mgos_zswitch_create("sw-1", NULL);
mgos_zswitch_gpio_attach(sw, 5, NULL);
}
```
### mgos_zswitch_gpio_detach()
```c
void mgos_zswitch_gpio_detach(struct mgos_zswitch *handle);
```
Detach the switch from the GPIO that was previously attached using `mgos_zswitch_gpio_attach()`.
## JS API Reference
### .GPIO.attach()
```js
let success = sw.GPIO.attach(pin, cfg);
```
Attach the switch to the GPIO. Returns `true` on success, `false` otherwise.

|Parameter|Type||
|--|--|--|
|pin|numeric|GPIO pin.|
|cfg|object|Optional. GPIO configuration. If missing, default configuration values are used.|

**GPIO configuration properties**
|Property|Type||
|--|--|--|
|*cfg*.activeHigh|boolean|Optional. `true` if switch is ON when GPIO output is high (1).|

**Example** - Create a switch using default configuration values and attach it to the GPIO 5.
```js
let sw = ZenSwitch.create('sw-1');
let success = sw.GPIO.attach(5, {activeHigh: true});
```
### .GPIO.detach()
```js
sw.GPIO.detach();
```
Detach the switch from the GPIO that was previously attached using `.GPIO.attach()`.
## Additional resources
Take a look to some other demo samples.

|Sample|Notes|
|--|--|
|[zswitch-demo](https://github.com/zendiy-mgos/zswitch-demo)|Mongoose OS demo firmware for using ZenSwitches.|
|[zswitch-mqtt-demo](https://github.com/zendiy-mgos/zswitch-mqtt-demo)|Mongoose OS demo firmware for using MQTT to drive ZenSwitches.|
