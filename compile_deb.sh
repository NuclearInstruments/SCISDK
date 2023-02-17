
cd build_x64/output
fpm \
  -s dir -t deb \
  -p ../../scisdk-x64.deb \
  --name scisdk \
  --license agpl3 \
  --version $CI_COMMIT_TAG \
  --architecture x86_64 \
  --description "Nuclear Instruments SciCompiler SDK" \
  --url "https://github.com/NuclearInstruments/SCISDK" \
  --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
  .

cd ../..

cd build_i386/output
fpm \
  -s dir -t deb \
  -p ../../scisdk-i386.deb \
  --name scisdk \
  --license agpl3 \
  --version $CI_COMMIT_TAG \
  --architecture i386 \
  --description "Nuclear Instruments SciCompiler SDK" \
  --url "https://github.com/NuclearInstruments/SCISDK" \
  --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
  .
cd ../..

cd build_arm64/output
fpm \
  -s dir -t deb \
  -p ../../scisdk-arm64.deb \
  --name scisdk \
  --license agpl3 \
  --version $CI_COMMIT_TAG \
  --architecture aarch64 \
  --description "Nuclear Instruments SciCompiler SDK" \
  --url "https://github.com/NuclearInstruments/SCISDK" \
  --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
  .
cd ../..

cd build_armhf/output
fpm \
  -s dir -t deb \
  -p ../../scisdk-armhf.deb \
  --name scisdk \
  --license agpl3 \
  --version $CI_COMMIT_TAG \
  --architecture armhf \
  --description "Nuclear Instruments SciCompiler SDK" \
  --url "https://github.com/NuclearInstruments/SCISDK" \
  --maintainer "Andrea Abba <abba@nuclearinstruments.eu>" \
  .
cd ../..
