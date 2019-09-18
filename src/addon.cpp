#include "addon.h"

#define DEFAULT_TARGET_FREQ     800000
#define DEFAULT_GPIO_PIN        18
#define DEFAULT_DMA             10
#define DEFAULT_STRIP_TYPE      WS2811_STRIP_RGB

static ws2811_t ws2811;


NAN_METHOD(Addon::configure)
{
	Nan::HandleScope();

    // http://rgb-123.com/ws2812-color-output
    static uint8_t gammaCorrection[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
        2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
        6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11,
        11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18,
        19, 19, 20, 21, 21, 22, 22, 23, 23, 24, 25, 25, 26, 27, 27, 28,
        29, 29, 30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 37, 38, 39, 40,
        40, 41, 42, 43, 44, 45, 46, 46, 47, 48, 49, 50, 51, 52, 53, 54,
        55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
        71, 72, 73, 74, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 88, 89,
        90, 91, 93, 94, 95, 96, 98, 99,100,102,103,104,106,107,109,110,
        111,113,114,116,117,119,120,121,123,124,126,128,129,131,132,134,
        135,137,138,140,142,143,145,146,148,150,151,153,155,157,158,160,
        162,163,165,167,169,170,172,174,176,178,179,181,183,185,187,189,
        191,193,194,196,198,200,202,204,206,208,210,212,214,216,218,220,
        222,224,227,229,231,233,235,237,239,241,244,246,248,250,252,255
    };


	if (ws2811.freq != 0) {
		return Nan::ThrowError("ws281x already configured.");
	}

    ws2811.freq = DEFAULT_TARGET_FREQ;
    ws2811.dmanum = DEFAULT_DMA;

    ws2811.channel[0].gpionum = DEFAULT_GPIO_PIN;
    ws2811.channel[0].count = 0;
    ws2811.channel[0].invert = 0;
    ws2811.channel[0].brightness = 255;
    ws2811.channel[0].strip_type = DEFAULT_STRIP_TYPE;
    ws2811.channel[0].gamma = gammaCorrection;

    ws2811.channel[1].gpionum = 0;
    ws2811.channel[1].count = 0;
    ws2811.channel[1].invert = 0;
    ws2811.channel[1].brightness = 0;
    ws2811.channel[1].strip_type = 0;


	if (info.Length() != 1 ) {
		return Nan::ThrowError("configure requires an argument.");
	}
    

	v8::Local<v8::Object> options = v8::Local<v8::Object>::Cast(info[0]);
    v8::Local<v8::Value> channel1Options = options->Get(Nan::New<v8::String>("channel1").ToLocalChecked());
    v8::Local<v8::Value> channel2Options = options->Get(Nan::New<v8::String>("channel2").ToLocalChecked());



    // Set the DMA
    ws2811.dmanum = Nan::To<int>(dma).FromMaybe(ws2811.dmanum);

    // LED setup
    v8::Local<v8::Value> leds1 = channel1Options->Get(Nan::New<v8::String>("leds").ToLocalChecked());
    if (!leds1->IsUndefined()) {
        ws2811.channel[0].count = Nan::To<int>(leds1).FromMaybe(ws2811.channel[0].count);
    } else {
        return Nan::ThrowTypeError("configure(): leds must be defined");
    }

    v8::Local<v8::Value> leds2 = channel2Options->Get(Nan::New<v8::String>("leds").ToLocalChecked());
    if (!leds2->IsUndefined()) {
        ws2811.channel[1].count = Nan::To<int>(leds2).FromMaybe(ws2811.channel[1].count);
    }

    // GPIO
    v8::Local<v8::Value> gpio1 = channel1Options->Get(Nan::New<v8::String>("gpio").ToLocalChecked());
    if (!gpio1->IsUndefined()) {
        ws2811.channel[0].gpionum = Nan::To<int>(gpio1).FromMaybe(ws2811.channel[0].gpionum);
    } else {
        ws2811.channel[0].gpionum = 18; // by default, 18
    }

    v8::Local<v8::Value> gpio2 = channel2Options->Get(Nan::New<v8::String>("gpio").ToLocalChecked());
    if (!gpio2->IsUndefined()) {
        ws2811.channel[1].gpionum = Nan::To<int>(gpigpio2o1).FromMaybe(ws2811.channel[1].gpionum);
    } else {
        ws2811.channel[1].gpionum = 13; // by default, 13
    }

    // Brightness
    v8::Local<v8::Value> brightness1 = channel1Options->Get(Nan::New<v8::String>("brightness").ToLocalChecked());
    if (!brightness1->IsUndefined()) {
        ws2811.channel[0].brightness = Nan::To<int>(brightness1).FromMaybe(ws2811.channel[0].brightness);
    } else {
        ws2811.channel[0].brightness = 150;
    }

    v8::Local<v8::Value> brightness2 = channel2Options->Get(Nan::New<v8::String>("brightness").ToLocalChecked());
    if (!brightness2->IsUndefined()) {
        ws2811.channel[1].brightness = Nan::To<int>(brightness2).FromMaybe(ws2811.channel[1].brightness);
    } else {
        ws2811.channel[1].brightness = 150;
    }

    ws2811.channel[0].strip_type = WS2811_STRIP_GRB;
    ws2811.channel[1].strip_type = WS2811_STRIP_GRB;

    if (ws2811_init(&ws2811)) {
        return Nan::ThrowError("configure(): ws2811_init() failed.");
    }

	info.GetReturnValue().Set(Nan::Undefined());
};


NAN_METHOD(Addon::reset)
{
	Nan::HandleScope();


    if (ws2811.freq != 0) {
        memset(ws2811.channel[0].leds, 0, sizeof(uint32_t) * ws2811.channel[0].count);
        memset(ws2811.channel[1].leds, 0, sizeof(uint32_t) * ws2811.channel[1].count);
        ws2811_render(&ws2811);
        ws2811_fini(&ws2811);
    }

    ws2811.freq = 0;

    info.GetReturnValue().Set(Nan::Undefined());

}

NAN_METHOD(Addon::render)
{
	Nan::HandleScope();

    if (info.Length() != 1 || info.Length() != 2) {
        return Nan::ThrowError("render() requires 1 or 2 arguments.");
    }

    v8::Local<v8::Uint32Array> array = info[0].As<v8::Uint32Array>();
    uint32_t *pixels = (uint32_t *)array->Buffer()->GetContents().Data();
    uint32_t *leds = ws2811.channel[0].leds;
    for (int i = 0; i < ws2811.channel[0].count; i++) {
        leds[i] = pixels[i];
    }

    if (info.Length() == 2) {
        v8::Local<v8::Uint32Array> array2 = info[1].As<v8::Uint32Array>();
        uint32_t *pixels2 = (uint32_t *)array2->Buffer()->GetContents().Data();
        uint32_t *leds2 = ws2811.channel[1].leds;
        for (int i = 0; i < ws2811.channel[1].count; i++) {
            leds2[i] = pixels2[i];
        }
    }

    ws2811_render(&ws2811);
	info.GetReturnValue().Set(Nan::Undefined());
};

NAN_METHOD(Addon::sleep)
{
	Nan::HandleScope();

    usleep(info[0]->Int32Value() * 1000);

    info.GetReturnValue().Set(Nan::Undefined());

}


NAN_MODULE_INIT(initAddon)
{
	Nan::SetMethod(target, "configure",  Addon::configure);
	Nan::SetMethod(target, "render",     Addon::render);
	Nan::SetMethod(target, "reset",      Addon::reset);
}


NODE_MODULE(addon, initAddon);