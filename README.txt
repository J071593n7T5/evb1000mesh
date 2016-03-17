DW1000 Application Programming Interface (API) package is composed of the following folders:
    - decadriver:
      Driver for DW1000 chip. Details about each function provided here can be found in DW1000 API Guide.
    - examples:
      A set of individual examples showing how to achieve different simple features (sending a frame, receiving a frame, putting the DW1000 to sleep, etc.).
    - Libraries, Linkers:
      Hardware abstraction layer (system start-up code and peripherals' drivers) for ARM Cortex-M and ST STM32 F1 processors. Provided by ST Microelectronics.
    - platform:
      Platform dependant implementation of low-level features (IT management, mutex, sleep, etc).

Please refer to DW1000 API Guide accompanying this package for more details about provided API and examples.

NOTE: The DW1000 API included in this package is an unbundled version of the DW1000 driver.
      This version may differ from that bundled with Decawave's other products.

=============================================================================
=                                                                           =
=                               Release Notes                               =
=                                                                           =
=============================================================================

=============================================================================
Package v1.01 / Driver v3.0.1  (3rd December 2015)
=============================================================================

a) Initial release of DW1000 Driver and API unbundled from other applications,
   with some simple example projects.

=============================================================================
