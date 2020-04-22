#!/bin/bash

ARM_FM_ENV=$ARM_FM_ROOT/source_all.sh
ARM_FM_SIMGEN=$ARM_FM_ROOT/bin/simgen
ARGS="$@"

echo "ARM FastModels Root: $ARM_FM_ROOT"

source $ARM_FM_ENV
echo "executing '$ARM_FM_SIMGEN $ARGS'"

$ARM_FM_SIMGEN $ARGS

