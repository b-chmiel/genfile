#!/bin/bash

set -euo pipefail

make deb-docker
DEB_FILENAME=genfile.deb

make dist
COMMIT_ID=$(git rev-list --max-count=1 HEAD)
RELEASE_NAME=1.0.5-dev-$COMMIT_ID
ARTIFACT_FILENAME=genfile-$RELEASE_NAME.tar.gz

pushd ./build_release
	mv genfile-1.0.5.tar.gz $ARTIFACT_FILENAME
	RELEASE_SHA512=$(sha512sum $ARTIFACT_FILENAME)

	gh release create $RELEASE_NAME $ARTIFACT_FILENAME $DEB_FILENAME \
		--generate-notes \
		--notes-file -<<EOF
\`\`\`
genfile_VERSION = $RELEASE_NAME

sha512  $RELEASE_SHA512
\`\`\`
EOF
popd
