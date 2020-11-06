#!/usr/bin/env bash

set -euo pipefail

PROJECT_DIR=$(
  cd "$(dirname "$0")"/..
  pwd
)

# identifies a Semver tag prefixed with 'refs/tags/v', e.g. "refs/tags/v1.2.3" -> "1.2.3".
regex='^refs\/tags\/v([[:digit:]]+\.[[:digit:]]+\.[[:digit:]]+)$'

# Extract Semver from Git tag.
echo "Tag: ${CNL_VERSION}"
if [[ "${CNL_VERSION}" =~ $regex ]]
then
    export version="${BASH_REMATCH[1]}"
    echo "CNL version ${version}"
else
    echo "Tag ${CNL_VERSION} not recognized as Semver version tag"
    exit 1
fi

# Push revision of documentation
pushd "${PROJECT_DIR}/doc/gh-pages"
git config --global user.email "github@john.mcfarlane.name"
git config --global user.name "John McFarlane (Travis)"
git remote set-url origin "https://johnmcfarlane:${GITHUB_TOKEN}@github.com/johnmcfarlane/cnl.git"
git reset origin/gh-pages
git checkout gh-pages
git add .
if git commit -m"Documentation v${version}"
then
  git push
fi
popd

# Upload new version of CNL Conan package to Bintray
conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl
conan user -p "${CONAN_PASS}" -r johnmcfarlane/cnl "${CONAN_USER}"
conan install --build=missing "${PROJECT_DIR}"
conan create . "cnl/${version}@johnmcfarlane/development"
conan upload "cnl/${version}@johnmcfarlane/development" -r johnmcfarlane/cnl
