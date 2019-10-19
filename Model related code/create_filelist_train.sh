# /usr/bin/env sh
DATA=data/train_zhiwu_sec
WORK=data
echo "Create train.txt..."
rm -rf $DATA/train.txt
find $DATA -name po_*.jpg | cut -d '/' -f3 | sed "s/$/ 1/">>$DATA/train.txt
find $DATA -name ne_*.jpg | cut -d '/' -f3 | sed "s/$/ 0/">>$DATA/tmp.txt
cat $DATA/tmp.txt>>$DATA/train.txt
rm -rf $DATA/tmp.txt
mv $DATA/train.txt $WORK/
echo "Done..."
