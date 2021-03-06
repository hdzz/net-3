
set(${PROJECT_NAME}_SOURCE_FILES
  buffered_socket.cpp 
  socket.cpp
  secure_layer.cpp
  socket_factory.cpp
  socket_server.cpp
  uri.cpp 
  telnet/socket.cpp 
  encoders.cpp
)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${OPENSSL_INCLUDE_DIR} SYSTEM PUBLIC "vendor/cereal/include")

target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_THREAD_LIBS_INIT} INTERFACE ${OPENSSL_LIBRARIES} INTERFACE ${URIPARSER_LIBRARIES})

set(${PROJECT_NAME}_HEADERS
  buffered_socket.h
  encoders.h
  exception.h
  secure_layer.h
  socket.h
  socket_factory.h
  socket_server.h
  socket_server_listener.h
  uri.h
)

string(REPLACE "_" "/" INSTALL_DIRECTORY ${PROJECT_NAME})

install(FILES ${${PROJECT_NAME}_HEADERS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${INSTALL_DIRECTORY}")

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

add_subdirectory(async)
add_subdirectory(http)
add_subdirectory(sync)
add_subdirectory(telnet)

