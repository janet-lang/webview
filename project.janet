(declare-project
  :name "webview"
  :author "Calvin Rose"
  :license "MIT"
  :url "https://github.com/janet-lang/webview"
  :repo "git+https://github.com/janet-lang/webview.git")

(defn shell
  "Helper for using pkg-config"
  [str]
  (def f (file/popen str))
  (def output (file/read f :all))
  (file/close f)
  (string/replace "\n" "" output))

(def webview-def (case (os/which)
  :windows "WEBVIEW_WINAPI"
  :macos "WEBVIEW_COCOA"
  "WEBVIEW_GTK"))

(defn parts
  "Split on whitespace."
  [str]
  (peg/match
    '{:ws (set " \t\n\r\0")
      :non-ws '(some (if-not :ws 1))
      :main (any (* (any :ws) :non-ws))}
    str))

(def more-flags (case (os/which)
  # :windows "-lole32 -lcomctl32 -loleaut32 -luuid -mwindows" # flags are for mingw
  :windows []
  :macos ["-framework" "WebKit"]
  (parts
    (shell `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`))))

(declare-native
    :name "webview"
    :cflags [;default-cflags ;more-flags]
    :defines {webview-def true}
    :source @["webview.c"])
