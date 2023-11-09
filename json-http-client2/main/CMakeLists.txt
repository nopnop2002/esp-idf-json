set(srcs "main.c")

idf_component_register(SRCS ${srcs}
	INCLUDE_DIRS "."
	EMBED_TXTFILES server_root_cert.pem)
