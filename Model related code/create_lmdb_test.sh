#!/usr/bin/env sh
DATA=data/test_zhiwu_sec
WORK=data
rm -rf $DATA/test_lmdb
/home/chad/caffe/build/tools/convert_imageset \
--shuffle \
--resize_height=28 \
--resize_width=28 \
/home/chad/caffe/armor/data/test_zhiwu_sec/ \
$WORK/test.txt \
$DATA/test_lmdb
