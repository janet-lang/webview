(import cook)

(def webview-def (case (os/which)
  :windows "WEBVIEW_WINAPI"
  :macos "WEBVIEW_COCOA"
  "WEBVIEW_GTK"))

(defn shell
  "Helper for using pkg-config"
  [str]
  (def f (file/popen str))
  (def output (file/read f :all))
  (file/close f)
  (string/replace "\n" "" output))

(def more-flags (case (os/which)
  # :windows "-lole32 -lcomctl32 -loleaut32 -luuid -mwindows" # flags are for mingw
  :windows ""
  :macos "-framework WebKit"
  (shell `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`)))

(cook/make-native
    :name "webview"
    :cflags (string cook/CFLAGS " " more-flags)
    :defines {webview-def true}
    :source @["webview.c"])
