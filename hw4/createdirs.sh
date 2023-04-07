#!/bin/bash

#create top level dir
mkdir -p maindir

#create sub dirs
mkdir -p maindir/subdir1
mkdir -p maindir/subdir2

#create files
truncate -s 1K maindir/file1.txt
truncate -s 3K maindir/file2.txt

truncate -s 10K maindir/subdir1/file3.txt
truncate -s 2K maindir/subdir1/file4.txt
truncate -s 4K maindir/subdir1/file5.txt
truncate -s 5K maindir/subdir1/file6.txt

truncate -s 7K maindir/subdir2/file7.txt
truncate -s 12K maindir/subdir2/file8.txt
truncate -s 15K maindir/subdir2/file9.txt
truncate -s 6K maindir/subdir2/file10.txt

#remove write permissions from half the files
chmod -w maindir/file1.txt

chmod -w maindir/subdir1/file3.txt
chmod -w maindir/subdir1/file4.txt

chmod -w maindir/subdir2/file7.txt
chmod -w maindir/subdir2/file8.txt