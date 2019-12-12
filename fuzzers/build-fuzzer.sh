#!/bin/bash -eu
# Copyright 2018 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################

./bootstrap.sh

# Limit the size of buffer allocations to avoid bogus OOM issues
# https://github.com/libgd/libgd/issues/422
sed -i'' -e 's/INT_MAX/100000/' "$SRC/libgd/src/gd_security.c"

./configure --prefix="$WORK" --disable-shared
make -j$(nproc) install

cd $SRC/libgd/fuzzers
# build fuzzer target
fuzzerFiles=$(find $SRC/libgd/fuzzers/ -name "*.cc")

# Assemble a corpus
curl -Lo afl_testcases.tgz http://lcamtuf.coredump.cx/afl/demo/afl_testcases.tgz
mkdir afl_testcases
(cd afl_testcases; tar xvf "$SRC/libgd/fuzzers/afl_testcases.tgz")

for file in $fuzzerFiles; do
    fuzzerName=$(basename $file .cc)
    echo "Building fuzzer $fuzzerName"
    for target in Bmp Gd Gd2 Gif Jpeg Png Tga Tiff WBMP Webp; do
         lowercase=$(echo $target | tr "[:upper:]" "[:lower:]")
         $CXX $CXXFLAGS -std=c++11 -I"$WORK/include" -L"$WORK/lib" \
           -DFUZZ_GD_FORMAT=$target \
           $file -o $OUT/${fuzzerName}_${lowercase}_target \
           $LIB_FUZZING_ENGINE -lgd -Wl,-Bstatic -lz -Wl,-Bdynamic
   done
done

#Add test cases to the fuzzing corpus
for format in bmp gif png webp; do
	for file in $fuzzerFiles; do
		fuzzerName=$(basename $file .cc)
		mkdir $format
		find afl_testcases -type f -name '*.'$format -exec mv -n {} $format/ \;
		zip -rj $format.zip $format/
		cp $format.zip "$OUT/${fuzzerName}_{format}_target_seed_corpus.zip"
	done
done
