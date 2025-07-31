# Step 9: Run Harvey OS in QEMU (Cross-Compiled Image)

After successfully cross-compiling Harvey OS for a target architecture (e.g., ARM64 in Step 8), you can run this version using the QEMU system emulator for that specific architecture. This allows you to test your cross-compiled build even if you don't have physical hardware for the target.

The `setup-harvey-dev-env.sh` script should have installed the necessary QEMU system emulators (e.g., `qemu-system-aarch64` for ARM64).

## 9.1. Prerequisites

*   **Harvey OS Cross-Compiled:** You must have completed Step 8 and have a successful cross-compiled build of Harvey OS for your target architecture.
*   **QEMU System Emulator Installed:** `setup-harvey-dev-env.sh` should have installed the relevant QEMU package (e.g., `qemu-system-arm` which often includes `qemu-system-aarch64`). Verify with, for example, `qemu-system-aarch64 --version`.
*   **Artifact Locations:** You need to know the location of your cross-compiled kernel and any necessary disk image or initramfs for that architecture.
    *   Example kernel path (for ARM64): `$HARVEY_ROOT/src/harvey-os/sys/src/9/arm64/9pi` (or similar, specific to Harvey's ARM64 build).
    *   Disk Image: Harvey OS might use a generic disk image for different architectures or require an architecture-specific one.
    *   **Note:** These paths are illustrative. Consult Harvey OS's specific documentation or build output for the correct paths.

*   **Current Directory:** It's convenient to run QEMU commands from the root of the Harvey OS source tree (`$HARVEY_ROOT/src/harvey-os/`).

## 9.2. Basic QEMU Command for Cross-Compiled Targets

The QEMU command will be specific to the target architecture. For example, for ARM64 (AArch64):

```bash
qemu-system-aarch64 [options] -kernel <path_to_arm64_kernel> -hda <path_to_disk_image_if_needed>
```

Key options often include:
*   `qemu-system-aarch64`: The QEMU command for AArch64 system emulation.
*   `-M <machine_type>`: Specifies the machine model to emulate (e.g., `-M virt` for a generic virtual platform, `-M raspi3b` for a Raspberry Pi 3 Model B). `virt` is often a good default for general ARM64 OS images.
*   `-cpu <cpu_type>`: Specifies the CPU to emulate (e.g., `-cpu cortex-a57`, `-cpu cortex-a72`).
*   `-kernel <path>`: Path to the ARM64 kernel.
*   `-drive file=<path>,format=raw,if=sd` (or `if=virtio`): For an SD card image or VirtIO block device.
*   `-m <size>`: RAM for the VM (e.g., `-m 1G`).
*   `-nographic`: Often used for ARM targets, as graphical output might not be configured by default, and interaction happens via simulated serial console.
*   `-append "<boot_arguments>"`: Kernel command line arguments.

**Harvey OS Specifics:**
Again, Harvey OS might have preferred QEMU invocations or run scripts for its supported cross-compiled targets. Check for scripts like `run_qemu_arm64.sh` or similar in the source tree or documentation.

## 9.3. Running Harvey OS (ARM64 Example)

Assuming your cross-compiled build is for ARM64:

1.  **Navigate to Harvey OS Source Directory:**
    ```bash
    cd $HARVEY_ROOT/src/harvey-os
    ```

2.  **Execute the QEMU Command:**
    Replace placeholder paths and options with those appropriate for your Harvey OS ARM64 build.
    ```bash
    # Example QEMU command for ARM64 (ADAPT THIS TO HARVEY OS SPECIFICS):
    qemu-system-aarch64 \
        -M virt \
        -cpu cortex-a57 \
        -kernel sys/src/9/arm64/9pi \
        # Add -drive option here if a disk image is needed for ARM64 boot
        # e.g., -drive file=disk-arm64.img,format=raw,if=virtio
        -m 1G \
        -nographic
        # With -nographic, output and input will be on your host terminal.
    ```
    If Harvey OS provides a specific run script for ARM64 (e.g., from the `scripts/` directory), use that:
    ```bash
    # Example if such a script exists:
    # ./scripts/run_qemu_arm64.sh
    ```

## 9.4. Interacting with the Cross-Compiled Harvey OS

*   With `-nographic`, QEMU will use your current terminal for serial console output from the guest.
*   You should see Harvey OS boot messages specific to the ARM64 architecture.
*   Eventually, you should reach a login prompt or be logged in, similar to the native boot.
*   Try basic commands to confirm functionality.

## 9.5. Exiting QEMU (for `-nographic`)

*   Typically, `Ctrl-a x` will exit QEMU when using `-nographic` and a serial console.
*   Alternatively, `Ctrl-a c` to switch to the QEMU monitor, then type `quit` or `q`.
*   A clean shutdown command from within the guest Harvey OS is always preferred if available.

## 9.6. Troubleshooting

*   **QEMU command not found (e.g., `qemu-system-aarch64`):** Ensure the correct QEMU system emulator for the target architecture was installed in Step 2.
*   **Kernel image not found or incorrect format:** Verify the path to your cross-compiled kernel and ensure it's the correct file for the target architecture.
*   **VM doesn't boot:**
    *   Cross-compilation might have issues, or the build was incomplete.
    *   Incorrect QEMU machine type (`-M`), CPU type (`-cpu`), or other options. ARM emulation can be more sensitive to these than x86.
    *   Missing essential boot media (e.g., a required disk image or initramfs for the ARM target).
    *   Consult Harvey OS documentation for recommended QEMU settings for ARM64.
*   **Serial console issues with `-nographic`:**
    *   **Symptoms:** No output, garbled output, or inability to input commands.
    *   **Root Cause Analysis:** Incorrect serial device configuration in QEMU, or guest OS not using the expected serial port for console. Harvey OS (like Plan 9) typically uses the first serial port (`ttyS0` or `COM1`) as its primary console.
    *   **Resolution Protocol:** The `-nographic` option usually defaults to redirecting the first serial port to the host stdio. If Harvey OS uses a different serial port by default for its console on that architecture, you might need specific QEMU options like `-serial mon:stdio` (to multiplex monitor and serial) or ensure the guest kernel arguments point to the correct console device. Check any provided `run_qemu_<arch>.sh` scripts for specific serial configurations.

## 9.7. Architectural Alignment

Running a cross-compiled Harvey OS image in QEMU is a powerful validation of several architectural tenets:

*   **Multi-architecture Philosophy & Portability ([ARCHITECTURAL_MANIFESTO.md](../../ARCHITECTURAL_MANIFESTO.md#3-multi-architecture-philosophy)):** This directly demonstrates the success of Harvey OS's portability efforts and its ability to function on architectures different from the primary development ones. It's key to supporting Tier 2 or experimental Tier 3 architectures as per `docs/architecture/tier-criteria.md`.
*   **Validation of Cross-Compilers and Build System:** A successful boot and operation of a cross-compiled image in QEMU validates that the cross-compiler toolchain and the build system are producing correct, functional code for the target architecture.
*   **CI Strategy for Non-Native Targets ([docs/ci-strategy.md](../../docs/ci-strategy.md#33-testing-strategy)):** The process used in this step (QEMU emulation for a non-native target) is exactly how the CI system would test such architectures. This manual run helps confirm the feasibility of such CI loops.
*   **Enabling Broader Hardware Support:** By ensuring cross-compiled images run under emulation, Harvey OS lowers the barrier for developers to work on ports for hardware they may not physically possess, accelerating the path to supporting new physical devices.

## 9.8. Next Steps

You have now successfully built, cross-compiled, and run Harvey OS for both your native architecture and a different target architecture (ARM64) within QEMU. This is a major milestone!

Proceed to **Step 10: Basic Validation & Next Steps** to perform a few final checks and get pointers for further exploration.
