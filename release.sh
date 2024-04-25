#!/bin/bash
rm -rf release
mkdir -p release

cp -rf hybridation *.{hpp,cpp,txt,json} LICENSE release/

mv release score-addon-hybridation
7z a score-addon-hybridation.zip score-addon-hybridation
