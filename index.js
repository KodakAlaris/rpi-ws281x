let path = require("path");
let addon = require(path.join(__dirname, "build", "Release", "rpi-ws281x.node"));

class Module {
    constructor() {
        this.displayLedCount = 256;
        this.binLedCount = 6;
    }

    configure(options) {
        const {ledDisplayOptions, binLightOptions} = options;
        addon.configure(
            {
                ledDisplayOptions: ledDisplayOptions || {brightness: 150},
                binLightOptions: binLightOptions || {brightness: 150}
            }
        );
    }

    reset() {
        this.render();
        addon.reset();    
    }

    sleep(ms) {
        addon.sleep(ms);
    }

    render(ledDisplayPixels = new Uint32Array(this.displayLedCount), binLedPixels = new Uint32Array(this.binLedCount)) {
        addon.render(ledDisplayPixels, binLedPixels);
    }
}


module.exports = new Module();
