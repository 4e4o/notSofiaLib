#!/bin/sh

rm -rf umap_dump
mkdir umap_dump
cd umap_dump

for f in "/proc/umap"/*
do 
    fname="$(basename -- $f)"

    if [ "$fname" != "logmpp" ]
    then
        cat $f > ${fname}.dump
        echo $fname
    fi
done
