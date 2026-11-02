# Secure STM32 Firmware OTA Update

An embedded firmware project focused on building a secure **Firmware
Over-The-Air (FOTA)** update system around an STM32 microcontroller and
an ESP32 Wi-Fi module.

The project brings together low-level STM32 firmware development, UART
communication, bootloader design, Flash memory programming, and secure
firmware delivery concepts. The long-term objective is to allow an
STM32-based device to receive and validate firmware updates without
requiring a physical programming connection.

> **Project status:** Work in progress. The repository currently
> contains the STM32 firmware project and supporting ESP32 tools.
> Features should be considered implemented only when their code and
> tests are present in the repository.

## Project goals

-   Develop reusable low-level drivers for the STM32 platform.
-   Establish reliable UART communication between the STM32 and an ESP32
    module.
-   Build the STM32 bootloader and firmware-update flow.
-   Receive firmware images in chunks and program the STM32 internal
    Flash.
-   Validate a downloaded image before transferring execution to the
    application.
-   Explore secure firmware distribution using HTTPS/TLS and AWS
    services such as Amazon S3.

## System architecture

The intended update flow is:

``` text
        Firmware image hosted remotely
             (e.g. Amazon S3)
                      |
                  HTTPS/TLS
                      |
                ESP32 Wi-Fi
                      |
                    UART
                      |
              STM32 bootloader
                      |
          Receive and buffer firmware
                      |
             Program internal Flash
                      |
             Validate firmware image
                      |
             Start application image
```

The ESP32 acts as the network interface, while the STM32 is responsible
for bootloader operation, Flash programming, and application validation.

## Hardware and software

  Component        Role
  ---------------- --------------------------------------------------
  STM32F429ZI      Main microcontroller and firmware-update target
  ESP32-WROOM-32   Wi-Fi connectivity and communication bridge
  UART             Communication between the STM32 and ESP32
  STM32CubeIDE     Embedded development environment
  Embedded C       STM32 firmware implementation
  AWS S3           Planned firmware-image hosting/distribution
  HTTPS/TLS        Secure transport concepts for firmware retrieval

## Current firmware modules

The STM32 firmware is organized around reusable peripheral and support
modules. The current development includes:

  -----------------------------------------------------------------------
  Module                              Purpose
  ----------------------------------- -----------------------------------
  UART driver                         UART communication for the debug
                                      interface and slave/ESP32 device

  Circular buffers                    Buffered TX and RX data handling

  Timebase                            Timing and delay support

  FPU                                 Floating-point unit
                                      configuration/support

  BSP                                 Board support and hardware-level
                                      initialization
  -----------------------------------------------------------------------

### UART communication

The UART layer is being developed for two communication endpoints:

-   **Debug UART:** used for debugging and exchanging messages with a
    terminal.
-   **Slave-device UART:** used for communication with the external
    device, such as the ESP32.

The driver uses interrupt-driven reception and transmission with
circular buffers:

-   The RX interrupt reads received data and places it in the RX
    circular buffer.
-   Application code retrieves received bytes from the buffer.
-   Application code places outgoing bytes in the TX circular buffer.
-   The TX interrupt moves queued bytes to the UART data register.
-   TX and RX buffers allow application code and interrupt handlers to
    process data asynchronously.

The circular-buffer implementation uses `head` and `tail` indices. With
one slot reserved to distinguish a full buffer from an empty one, a
buffer of size 100 can store up to 99 bytes.

## Repository structure

``` text
Secure-STM32-Firmware-OTA-Update/
│
├── FOTA-firmware/
│   ├── Inc/                 # Firmware header files
│   ├── Src/                 # Firmware source files
│   ├── Startup/             # STM32 startup code
│   ├── *.ld                 # Linker scripts
│   └── ...                  # STM32CubeIDE project files
│
├── ESP32-tools/             # ESP32 tools and supporting documentation
│
├── chip_headers/            # MCU/CMSIS device header files
│
├── .gitignore
└── README.md
```

The exact contents may evolve as the project develops.

## Planned OTA update workflow

The target firmware-update sequence is:

1.  The STM32 starts and executes the bootloader.
2.  The bootloader determines whether an update or normal application
    startup is required.
3.  The STM32 communicates with the ESP32 over UART.
4.  The ESP32 connects to Wi-Fi and retrieves the firmware image from
    the remote server over HTTPS.
5.  The ESP32 transfers the firmware data to the STM32 in chunks.
6.  The STM32 receives the data and programs the designated Flash memory
    region.
7.  The bootloader checks the received image before accepting it.
8.  If validation succeeds, the STM32 starts the updated application. If
    validation fails, the bootloader should preserve a safe recovery
    path.

This describes the intended design; individual steps should be marked
complete only after implementation and testing.

## Security considerations

A reliable OTA system must protect both firmware transport and firmware
authenticity.

-   **HTTPS/TLS** protects data in transit between the ESP32 and the
    firmware server.
-   **Image validation** checks that the received firmware is complete
    and structurally valid.
-   **CRC/checksum** can detect accidental corruption during transfer or
    storage.
-   **Digital signature verification** is recommended to authenticate
    firmware origin and integrity. A CRC alone does not prove that
    firmware came from a trusted publisher.
-   **Recovery handling** should prevent an interrupted or invalid
    update from leaving the device unable to boot.

Never commit Wi-Fi passwords, AWS access keys, private signing keys, or
other credentials to this repository.

## Development roadmap

Update this checklist as implementation and testing progress:

-   [x] STM32F429ZI project setup
-   [x] Initial STM32 firmware structure
-   [x] UART driver development for debug and slave-device communication
-   [x] TX/RX circular-buffer implementation
-   [x] Timebase, FPU, and BSP module development
-   [ ] Complete and test UART communication with the ESP32
-   [ ] Implement the STM32 bootloader update state machine
-   [ ] Implement Flash erase and programming routines
-   [ ] Implement firmware-image validation
-   [ ] Configure ESP32 Wi-Fi and HTTPS communication
-   [ ] Configure remote firmware hosting (e.g. Amazon S3)
-   [ ] Integrate end-to-end OTA update flow
-   [ ] Test successful updates, corrupted images, and interrupted
    updates
-   [ ] Add authenticated firmware verification

## Building the STM32 firmware

1.  Clone the repository:

    ``` bash
    git clone https://github.com/Melek-73/Secure-STM32-Firmware-OTA-Update-with-AWS-SSL-TLS-Connection.git
    ```

2.  Open the STM32 project contained in `FOTA-firmware/` using
    STM32CubeIDE.

3.  Check that the selected target and project configuration match the
    STM32F429ZI hardware.

4.  Build the project from STM32CubeIDE.

Build instructions may need to be updated if the project configuration
or directory structure changes.

## Scope and status

This repository is an evolving embedded-systems project. The current
focus is on the STM32 firmware foundation, including peripheral drivers
and buffered UART communication. The complete secure OTA
workflow---including ESP32 HTTPS retrieval, AWS-hosted firmware
distribution, bootloader integration, and end-to-end validation---is a
development objective and should not be assumed complete until the
corresponding implementation and tests are available.

## Author

**Melek Fourati**\
Final-year ICT Engineering Student --- SUP'COM, Tunisia

Interests: Embedded Systems, STM32 Firmware Development, Embedded Linux,
FPGA, and Edge AI.
