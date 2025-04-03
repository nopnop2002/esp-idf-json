idf_component_register(SRCS "main.c" "web_server.c" "web_client.c" "http_get_notify.c"
    INCLUDE_DIRS "."
    EMBED_FILES "../html/error.html"
                "../html/favicon.ico"
                "../html/main.js"
                "../html/root.html"
                "../html/main.css")
