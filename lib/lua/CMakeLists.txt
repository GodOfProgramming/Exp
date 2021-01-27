set(SRC_DIR upstream)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "${SRC_DIR}/*.c")

add_library(lua
  ${SRC_FILES}
)
