set(req driver freertos log esp_idf_lib_helpers esp_event esp_common)

idf_component_register(
    SRCS adxl345.c
    INCLUDE_DIRS .
    REQUIRES ${req}
)
