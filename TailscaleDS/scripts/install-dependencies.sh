#!/bin/bash

# Step 1: Download and Install MSYS2
# ------------------------------------
# You need to manually download and install MSYS2 from https://www.msys2.org/
# Follow the installation instructions on the website and update the package database.

# Once MSYS2 is installed, continue with the next steps by running this script from the MSYS2 shell.

# Step 2: Update MSYS2
# ------------------------------------
# Open the MSYS2 MinGW 64-bit terminal (search in the start menu) and run the following commands.

# Update the package database and core system packages.
pacman -Syu --noconfirm

# Close the terminal and reopen it for changes to take effect, then run the next update command.
pacman -Su --noconfirm

# Step 3: Install Development Tools (make, gcc)
# ----------------------------------------------
# Install base development tools (including make and gcc).
pacman -S --noconfirm base-devel mingw-w64-x86_64-toolchain

# Verify installation of gcc
gcc --version

# Step 4: Install DevkitARM
# ----------------------------------------------
# Install devkitARM using the pacman package manager.
pacman -S --noconfirm devkitARM

# Step 5: Add DevkitARM to the PATH
# ----------------------------------------------
# Add DevkitARM to your PATH by modifying .bashrc.
echo 'export DEVKITARM=/opt/devkitPro/devkitARM' >> ~/.bashrc
echo 'export PATH=$DEVKITARM/bin:$PATH' >> ~/.bashrc

# Apply the changes to the current shell.
source ~/.bashrc

# Step 6: Verify the Installation
# ----------------------------------------------
# Check if the ARM compiler is working correctly by verifying its version.
arm-none-eabi-gcc --version

# Step 7: Install Windows-Build-Tools (Optional for Node.js Development)
# ----------------------------------------------
# Install windows-build-tools globally using npm (if you're working with Node.js).
npm install -g windows-build-tools

# This installs Python 2.7 and Visual Studio Build Tools, required for building native Node.js modules.

# Final Note: After completing all steps, you should have pacman, devkitARM, gcc, and other development tools installed on your Windows machine.
