set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/ragel_gen)

file(MAKE_DIRECTORY ${OUTPUT_DIR})

file(GLOB RAGEL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.rl")
MESSAGE(STATUS "Ragel files: ${RAGEL_FILES}")

file(GLOB local_source
  *.h
  *.cc
)

set(GENERATED_FILES "")
foreach(RAGEL_FILE ${RAGEL_FILES})
  get_filename_component(FILENAME_WE ${RAGEL_FILE} NAME_WE)

  set(GENERATED_FILE ${OUTPUT_DIR}/${FILENAME_WE}.h)

  add_custom_command(
    OUTPUT ${GENERATED_FILE}
    COMMAND ragel -G2 ${RAGEL_FILE} -C -o ${GENERATED_FILE}
    DEPENDS ${RAGEL_FILE}
    COMMENT "Generating ${GENERATED_FILE} from ${RAGEL_FILE}"
  )

  list(APPEND GENERATED_FILES ${GENERATED_FILE})
endforeach()

add_library(common_ragel ${local_source} ${GENERATED_FILES})
set_target_properties(common_ragel PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(common_ragel PRIVATE ${CMAKE_BINARY_DIR}/src/common/ragel/ragel_gen)