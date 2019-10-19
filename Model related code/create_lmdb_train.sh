#!/usr/bin/env sh
DATA=data/train_zhiwu_sec
WORK=data
rm -rf $DATA/train_lmdb
/home/chad/caffe/build/tools/convert_imageset \
--shuffle \
--resize_height=28 \
--resize_width=28 \
/home/chad/caffe/armor/data/train_zhiwu_sec/  \
$WORK/train.txt \
$DATA/train_lmdb
