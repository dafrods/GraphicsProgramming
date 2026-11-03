DEST="$PROJECTS/openGL/dump/helloworld.exe"

cp $HOME/openGL/proj/build/helloworld.exe $DEST

if [ -e $DEST ]; then
    echo "COPY OK"
    exit 0 # GOOD
fi

echo "ERROR COPY"
exit 1

