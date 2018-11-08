MOUNTING_POINT=${1:-mnt}
IMAGE=${2:-../resources/fatfs.img}

make clean
make
echo "-------- TEST: Mounting the $IMAGE file system on $MOUNTING_POINT"
echo "$IMAGE ./$(basename $IMAGE)"
cp -f $IMAGE ./$(basename $IMAGE)
./fat-fuse ./$(basename $IMAGE) $MOUNTING_POINT

mapfile -t files < <( ls $MOUNTING_POINT )

clean_and_exit () {
  echo "-------- TEST: Exiting"
  sleep 1  # Make sure it finished mounting
  fusermount -u $MOUNTING_POINT
  rm ./$(basename $IMAGE)
  exit $1
}

if [ ${#files[@]} -eq 0 ]
then
  echo "-------- ERROR: Empty image"
  clean_and_exit -1
fi

echo "-------- VFS content"
for item in ${files[*]}
do
    echo "---------------- $item"
done

if [ ${#files[@]} \> 1 ] || [ "${files[0]}" != "MANIF.TXT" ]
then
  echo "-------- ERROR: Use the original image with only MANIF.TXT"
  clean_and_exit -1
fi

# Test if we can read the file
counts=$(grep "Marx" $MOUNTING_POINT/MANIF.TXT | wc -l)
if [ "$counts" == "1" ]
then
  echo "-------- TEST: Reading file OK"
else
  echo "-------- ERROR: Not reading file correctly"
  clean_and_exit -1
fi

echo "-------- TEST: Congratulations, you didn't break anything!"

echo "-------- TEST: Testing censorship"

TESTNAME="Reading file with censorship"
counts=$(grep "MANIFESTO OF THE xxxxxxxxx PARTY" $MOUNTING_POINT/MANIF.TXT | wc -l)
if [ "$counts" == "1" ]
then
  echo "---------------- PASSED: $TESTNAME OK"
else
  echo "---------------- ERROR: $TESTNAME not working for word COMMUNIST"
  clean_and_exit -1
fi

echo "-------- TEST: Writing"

TESTNAME="Create files"
touch $MOUNTING_POINT/somefile
exit_code=$?
if [ $exit_code -eq 0 ]
then
  echo "---------------- PASSED: $TESTNAME OK"
else
  echo "---------------- ERROR: $TESTNAME not working"
  clean_and_exit -1
fi

TESTNAME="Write files with one cluster"
DATA="Enough data for cluster 1"
echo $DATA > $MOUNTING_POINT/newfile2
counts=$(grep "$DATA" $MOUNTING_POINT/newfile2 | wc -l)
if [ "$counts" == "1" ]
then
  echo "---------------- PASSED: $TESTNAME OK"
else
  echo "---------------- ERROR: $TESTNAME not working"
  clean_and_exit -1
fi

TESTNAME="Write files with multiple clusters"
yes | head -n 1024 > $MOUNTING_POINT/newfile3
counts=$(grep "y" $MOUNTING_POINT/newfile3 | wc -l)
if [ "$counts" == "1024" ]
then
  echo "---------------- PASSED: $TESTNAME OK"
else
  echo "---------------- ERROR: $TESTNAME not working"
  clean_and_exit -1
fi

TESTNAME="Write files in sub-directories"
DATA="Writing in sub dirs"
mkdir $MOUNTING_POINT/newdir
echo $DATA > $MOUNTING_POINT/newdir/newfile2
counts=$(grep "$DATA" $MOUNTING_POINT/newdir/newfile2 | wc -l)
if [ "$counts" == "1" ]
then
  echo "---------------- PASSED: $TESTNAME OK"
else
  echo "---------------- ERROR: $TESTNAME not working"
  clean_and_exit -1
fi


echo "-------- TEST: All test passed"

clean_and_exit 0
