INCLUDEPATH += \
    $$PWD/sql \
    $$PWD/tables

DEPENDPATH += \
    $$PWD/sql \
    $$PWD/tables

HEADERS += \
    $$PWD/sql/mysql.h \
    $$PWD/sql/mysql_table.h \
    $$PWD/sql/sqlconnectpool.h \
    $$PWD/tables/ag_device_control_table.h \
    $$PWD/tables/ag_emu_extra_property_table.h \
    $$PWD/tables/ag_emu_mi_table.h \
    $$PWD/tables/ag_emu_property_table.h \
    $$PWD/tables/ag_emu_status_table.h \
    $$PWD/tables/ag_gateway_data_table.h \
    $$PWD/tables/ag_mi_cmd_table.h \
    $$PWD/tables/ag_mi_extra_property_table.h \
    $$PWD/tables/ag_mi_property_table.h \
    $$PWD/tables/ag_mi_report_table.h \
    $$PWD/tables/ag_ota_file_table.h \
    $$PWD/tables/ag_pos_table.h \
    $$PWD/tables/ag_power_data_table.h \
    $$PWD/tables/ag_power_index_table.h \
    $$PWD/tables/ag_rack_data_table.h \
    $$PWD/tables/ag_rack_extra_data_table.h \
    $$PWD/tables/ag_rack_index_table.h \
    $$PWD/tables/ag_rack_mi_table.h \
    $$PWD/tables/ag_station_emu_table.h \
    $$PWD/tables/ag_temp_table.h \
    $$PWD/tables/ag_user_act_table.h \
    $$PWD/tables/ag_user_station_table.h \
    $$PWD/tables/ag_user_table.h \
    $$PWD/tables/ag_workorder_table.h


SOURCES += \
    $$PWD/sql/mysql.cpp \
    $$PWD/sql/mysql_table.cpp \
    $$PWD/sql/sqlconnectpool.cpp \
    $$PWD/tables/ag_device_control_table.cpp \
    $$PWD/tables/ag_emu_extra_property_table.cpp \
    $$PWD/tables/ag_emu_mi_table.cpp \
    $$PWD/tables/ag_emu_property_table.cpp \
    $$PWD/tables/ag_emu_status_table.cpp \
    $$PWD/tables/ag_gateway_data_table.cpp \
    $$PWD/tables/ag_mi_cmd_table.cpp \
    $$PWD/tables/ag_mi_extra_property_table.cpp \
    $$PWD/tables/ag_mi_property_table.cpp \
    $$PWD/tables/ag_mi_report_table.cpp \
    $$PWD/tables/ag_ota_file_table.cpp \
    $$PWD/tables/ag_pos_table.cpp \
    $$PWD/tables/ag_power_data_table.cpp \
    $$PWD/tables/ag_power_index_table.cpp \
    $$PWD/tables/ag_rack_data_table.cpp \
    $$PWD/tables/ag_rack_extra_data_table.cpp \
    $$PWD/tables/ag_rack_index_table.cpp \
    $$PWD/tables/ag_rack_mi_table.cpp \
    $$PWD/tables/ag_station_emu_table.cpp \
    $$PWD/tables/ag_temp_table.cpp \
    $$PWD/tables/ag_user_act_table.cpp \
    $$PWD/tables/ag_user_station_table.cpp \
    $$PWD/tables/ag_user_table.cpp \
    $$PWD/tables/ag_workorder_table.cpp
