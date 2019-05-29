# webview

[Janet](https://janet-lang.org) bindings to the [webview](https://github.com/zserge/webview) project.
Use an embedded browser view to build UI with minimal dependencies.

## Building

To build, one must have janet installed on their system.

So far, these bindings have only been tested on linux, but should work on macos and
windows with some debugging.

Use the build.janet script to build the file `webview.so` on macos and linux, and `webview.dll`
on windows.

```
jpm build
```

## Testing

Run a sample application that opens Hacker News with

```
jpm test
```

## API

The API pretty closely matches the C API for webview. See the C api for more information.

### `(webview/init & options)`

Create a new webview handle. The options are:
- :title - string
- :width - integer
- :height - integer
- :url - string
- :debug - boolean
- :resizable - boolean

### `(webview/loop handle [,blocking])`

Process on event in a loop. Typical usage looks like

```
(while (webview/loop my-handle) nil)
```

### `(webview/set-title handle title)`

Set the title of the webview.

### `(webview/exit handle)`

Closes the window.

### `(webview/terminate handle)`

Exists the webview loop.

### `(webview/debug str)`

Prints a string to the appropriate debug console.

### `(webview/eval handle javascript)`

Evaluate some JavaScript inside the webview browser.
