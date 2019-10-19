# /usr/bin/env sh
DATA=data/test_zhiwu_sec
WORK=data
echo "Create test.txt..."
rm -rf $WORK/test.txt
find $DATA -name po_*.jpg | cut -d '/' -f3 | sed "s/$/ 1/">>$DATA/test.txt
find $DATA -name ne_*.jpg | cut -d '/' -f3 | sed "s/$/ 0/">>$DATA/tmp.txt
cat $DATA/tmp.txt>>$DATA/test.txt
rm -rf $DATA/tmp.txt
mv $DATA/test.txt $WORK/
echo "Done..."
