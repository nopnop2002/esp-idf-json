set(srcs "main.c")

idf_component_register(SRCS ${srcs}
	INCLUDE_DIRS "."
	EMBED_TXTFILES metaweather_com_root_cert.pem)
