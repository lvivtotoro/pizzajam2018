message(STATUS "downloading...
     src='http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz'
     dst='/home/midnightas/cpp/pizzajam/ogg-prefix/src/libogg-1.3.1.tar.gz'
     timeout='none'")




file(DOWNLOAD
  "http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz"
  "/home/midnightas/cpp/pizzajam/ogg-prefix/src/libogg-1.3.1.tar.gz"
  SHOW_PROGRESS
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
