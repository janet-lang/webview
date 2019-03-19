#define WEBVIEW_IMPLEMENTATION
#include "webview.h"
#include "janet.h"

#if defined(JANET_WINDOWS)
#pragma comment( lib, "gdi32" )
#pragma comment( lib, "advapi32" )
#endif

JanetAbstractType WV = {
    "webview/view",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

/* Get an integer from the option table */
static int opt_int(JanetTable *opts, const char *key, int default_value) {
    Janet keyword = janet_ckeywordv(key);
    Janet check = janet_table_get(opts, keyword);
    if (janet_checkint(check))
        return janet_unwrap_integer(check);
    if (janet_checktype(check, JANET_NIL))
        return default_value;
    janet_panicf("expected integer for option :%s, got %v", key, check);
    return 0;
}

/* Get an integer from the option table */
static const char * opt_string(JanetTable *opts, const char *key, const char *default_value) {
    Janet keyword = janet_ckeywordv(key);
    Janet check = janet_table_get(opts, keyword);
    if (janet_checktype(check, JANET_STRING))
        return (const char *) janet_unwrap_string(check);
    if (janet_checktype(check, JANET_NIL))
        return default_value;
    janet_panicf("expected string for option :%s, got %v", key, check);
    return "";
}

/* Check boolean option */
static int opt_bool(JanetTable *opts, const char *key, int default_value) {
    Janet keyword = janet_ckeywordv(key);
    Janet check = janet_table_get(opts, keyword);
    if (janet_checktypes(check, JANET_TFLAG_BOOLEAN))
        return janet_unwrap_boolean(check);
    if (janet_checktype(check, JANET_NIL))
        return default_value;
    janet_panicf("expected string for option :%s, got %v", key, check);
    return 0;
}

static Janet cfun_init(int32_t argc, Janet *argv) {
    if (argc & 1) janet_panic("expected even number of arguments");
    JanetTable *opts = janet_table(argc / 2);
    for (int32_t i = 0; i < argc; i += 2)
        janet_table_put(opts, argv[i], argv[i + 1]);

    const char *title = opt_string(opts, "title", "Janet Webview Application");
    const char *url = opt_string(opts, "url", "http://localhost");
    int width = opt_int(opts, "width", 800);
    int height = opt_int(opts, "height", 600);
    int resizable = opt_bool(opts, "resizable", 1);
    int debug = opt_bool(opts, "debug", 1);

    struct webview *wv = janet_abstract(&WV, sizeof(struct webview));
    wv->title = title;
    wv->url = url;
    wv->width = width;
    wv->height = height;
    wv->debug = debug;
    wv->resizable = resizable;
    webview_init(wv);
    return janet_wrap_abstract(wv);
}

static Janet cfun_loop(int32_t argc, Janet *argv) {
    janet_arity(argc, 1, 2);
    int blocking = 1;
    struct webview *wv = janet_getabstract(argv, 0, &WV);
    if (argc == 2) {
        blocking = janet_getboolean(argv, 1);
    }
    return janet_wrap_boolean(webview_loop(wv, blocking) == 0);
}

static Janet cfun_exit(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    struct webview *wv = janet_getabstract(argv, 0, &WV);
    webview_exit(wv);
    return argv[0];
}

static Janet cfun_set_title(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    struct webview *wv = janet_getabstract(argv, 0, &WV);
    const char *str = (const char *) janet_getstring(argv, 1);
    webview_set_title(wv, str);
    return argv[0];
}

static Janet cfun_terminate(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    struct webview *wv = janet_getabstract(argv, 0, &WV);
    webview_terminate(wv);
    return argv[0];
}

static Janet cfun_debug(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    const char *str = (const char *) janet_getstring(argv, 0);
    webview_debug("%s", str);
    return argv[0];
}

static Janet cfun_eval(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    struct webview *wv = janet_getabstract(argv, 0, &WV);
    const char *str = (const char *) janet_getstring(argv, 1);
    webview_eval(wv, str);
    return argv[0];
}

/****************/
/* Module Entry */
/****************/

static const JanetReg cfuns[] = {
    {
        "init", cfun_init,
        "(webview/init & options)\n\n"
            "Initialize a new webview object. Returns a handle for webview which can be further manipulated."
    },
    {
        "loop", cfun_loop,
        "(webview/loop handle [,blocking])\n\n"
            "Get the next webview event. Returns handle."
    },
    {
        "exit", cfun_exit,
        "(webview/exit handle)\n\n"
            "Close a webview handle. This means closing the webview window."
    },
    {
        "set-title", cfun_set_title,
        "(webview/set-title handle title)\n\n"
            "Sets the title of the webview window. Returns the handle."
    },
    {
        "terminate", cfun_terminate,
        "(webview/terminate handle)\n\n"
            "Exits the main loop. Returns the handle."
    },
    {
        "debug", cfun_debug,
        "(webview/debug str)\n\n"
            "Prints a string to the console or debug output for the embedded browser. Returns str."
    },
    {
        "eval", cfun_eval,
        "(webview/eval handle javascript)\n\n"
            "Evaluate javascript inside the browser. Returns handle."
    },
    {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
    janet_cfuns(env, "webview", cfuns);
}
