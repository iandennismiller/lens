#!/bin/bash

export LENSDIR=/app/Lens-linux
export PATH=${PATH}:${LENSDIR}/Bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LENSDIR}/Bin

exec ${LENSDIR}/Bin/lens

