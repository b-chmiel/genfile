#!/bin/bash

set -euo pipefail

make dist
COMMIT_ID=$(git rev-list --max-count=1 HEAD)
RELEASE_NAME=1.0.5-dev-$COMMIT_ID
ARTIFACT_FILENAME=gen_file-$RELEASE_NAME.tar.gz

mv build/gen_file-*.tar.gz $ARTIFACT_FILENAME
RELEASE_SHA512=$(sha512sum $ARTIFACT_FILENAME)

gh release create $RELEASE_NAME $ARTIFACT_FILENAME \
	--generate-notes \
	--notes-file -<<EOF
GEN_FILE_VERSION = $RELEASE_NAME
sha512  $RELEASE_SHA512
EOF
