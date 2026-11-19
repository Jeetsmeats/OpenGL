#!/bin/bash
# Run on the RTX GPU instead of the integrated Intel GPU (hybrid laptop).
# Without these variables the app silently renders on the iGPU.
__NV_PRIME_RENDER_OFFLOAD=1 \
__GLX_VENDOR_LIBRARY_NAME=nvidia \
__EGL_VENDOR_LIBRARY_FILENAMES=/usr/share/glvnd/egl_vendor.d/10_nvidia.json \
./build/OPENGL "$@"
