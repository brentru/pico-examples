if (NOT PICO_NO_HARDWARE)
    add_subdirectory(serial_passthru)
    add_subdirectory(uart_advanced)
endif ()
