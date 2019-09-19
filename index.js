let path = require("path");
let addon = require(path.join(__dirname, "build", "Release", "rpi-ws281x.node"));

class Module {
    constructor() {
        this.displayLedCount = 256;
        this.binLedCount = 6;
    }

    configure(options) {
        addon.configure(options);
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
