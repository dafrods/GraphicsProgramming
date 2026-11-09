DEST="$PROJECTS/openGL/dump/"

# cp $HOME/openGL/proj/build/*.exe $DEST
rsync $HOME/openGL/proj/build $DEST -ac

if [ -e $DEST ]; then
    echo "COPY OK"
    exit 0 # GOOD
fi

echo "ERROR COPY"
exit 1

