basic_sensors.o basic_sensors.d : basic_sensors.c ../../src/kernel/include/nrk.h \
  ../../src/kernel/hal/include/nrk_cpu.h nrk_cfg.h \
  ../../src/kernel/include/nrk_time.h \
  ../../src/platform/firefly2_2/include/include.h \
  ../../src/platform/firefly2_2/include/hal.h \
  ../../src/platform/firefly2_2/include/hal_firefly2_2.h \
  ../../src/radio/cc2420/include/hal_cc2420.h \
  ../../src/radio/cc2420/hal/atmega1281/cc2420_mcu_hal.h \
  ../../src/radio/cc2420/platform/firefly2_2/cc2420_platform_hal.h \
  ../../src/radio/cc2420/include/basic_rf.h \
  ../../src/kernel/include/nrk_events.h ../../src/platform/include/ulib.h \
  ../../src/platform/firefly2_2/include/nrk_pin_define.h \
  ../../src/platform/firefly2_2/include/nrk_platform_time.h \
  ../../src/kernel/include/nrk_task.h \
  ../../src/kernel/include/nrk_error.h \
  ../../src/kernel/hal/include/nrk_timer.h \
  ../../src/drivers/include/nrk_driver_list.h \
  ../../src/kernel/include/nrk_driver.h \
  ../../src/drivers/platform/firefly2_2/include/ff_basic_sensor.h
