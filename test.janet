# A simple test application

(import build/webview :as webview)

(def wv (webview/init
                      :title "Hacker News"
                      :url "https://news.ycombinator.com"
                      :width 1000
                      :height 800
                      :resizable true
                      :debug false))

# Main loop
(while (webview/loop wv) nil)

(webview/terminate wv)
