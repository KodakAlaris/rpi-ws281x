var path = require("path");
var addon = require(path.join(__dirname, "build", "Release", "rpi-ws281x.node"));

class Module {
    constructor() {
        this.c1leds = undefined;
        this.c2leds = undefined;
    }

    configure(options) {
        let channel1 = options.channel1;
        let channel2 = options.channel2;

        if (channel1.leds === undefined) {
            throw new Error(`Channel 0 LEDs must be defined.`);
        }

        this.c1leds = channel1.leds;
        if (channel2.leds !== undefined) {
            this.c2leds = channel2.leds;
        }
        addon.configure({channel1, channel2});
    }

    reset() {
        if (this.c1leds !== undefined && this.c2leds === undefined) {
            this.render(new Uint32Array(this.c1leds), null);
        } else {
            this.render(new Uint32Array(this.c1leds), new Uint32Array(this.c2leds));
        }
        addon.reset();
    }

    sleep(ms) {
        addon.sleep(ms);
    }

    render(c1Pixels, c2Pixels = null) {
        if (c1Pixels && !c2Pixels) {
            addon.render(c1Pixels);
        } else if (c1Pixels && c2Pixels) {
            addon.render(c1Pixels, c2Pixels);
        }
    }
}


module.exports = new Module();
