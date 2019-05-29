(declare-project
  :name "webview"
  :author "Calvin Rose"
  :license "MIT"
  :url "https://github.com/janet-lang/webview"
  :repo "git+https://github.com/janet-lang/webview.git")

(def webview-def (case (os/which)
  :windows "WEBVIEW_WINAPI"
  :macos "WEBVIEW_COCOA"
  "WEBVIEW_GTK"))

(def more-flags (case (os/which)
  # :windows "-lole32 -lcomctl32 -loleaut32 -luuid -mwindows" # flags are for mingw
  :windows ""
  :macos "-framework WebKit"
  (shell `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`)))

(declare-native
    :name "webview"
    :cflags (string cook/CFLAGS " " more-flags)
    :defines {webview-def true}
    :source @["webview.c"])
