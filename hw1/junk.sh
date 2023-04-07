#!/bin/bash

readonly recyclingBin=~/.junk

function usage {
  cat << EOF
  Usage: junk.sh [-hlp] [list of files]
   -h: Display help.
   -l: List junked files.
   -p: Purge all files.
   [list of files] with no other arguments to junk those files.
EOF
}

flag=0
list=0
purge=0

checkFlag() {
   if [[($FLAG > 0)]]; 
    then exit_abnormal
   fi
}

exit_abnormal() {                         # Function: Exit with error.
  echo "Error: Too many options enabled." >&2
  usage
  exit 1
}

while getopts ":hlp" options: do

    case "${options}" in
    h)
      if [[($# > 1)]]; 
      then exit_abnormal
      fi
      checkFlag                          
      usage
      flag=$(($flag + 1))
      exit 0
      ;;

    l)
      if [[($# > 1)]]; then
       exit_abnormal
      fi
      checkFlag                                        
      flag=$(($flag + 1))
      list=2
      ;;

    p)
      if [[($# > 1)]]; then
       exit_abnormal
      fi
      checkFlag
      flag=$(($flag + 1))
      purge=3
    ;;
    *)
      echo $'Error: Unknown argument \'-'$OPTARG$'\'.' >&2
      usage
      exit 1
    ;;
  esac
done

if [[($# == 0)]]; then
   usage                        #check for no arguments
   exit 0
fi

shift "$((OPTIND-1))"           #clear arguments for script running

#searching for files/purging/etc etc

if [ ! -d "$recyclingBin" ]; then                       #create recycling bin
	mkdir "$recyclingBin"                
	exit 0
fi

junkSearch(){
 ls -lAF "$recyclingBin"                                #search recycling bin
 exit 0
}

junkPurge(){
  rm -rf $recyclingBin/* $recyclingBin/.* 2>/dev/null     #empty recycling bin
  exit 0
}

 if [[($list > 0)]]; then
   junkSearch
 fi

 if [[($purge > 0)]]; then
   junkPurge
 fi

location=0

for i in $@; do
	filenames[$location]="$i"
	((++index))
done

for n in ${filenames[@]}; do
	if [ ! -e "$n" ] ; then
		printf "Warning: '$n' not found. \n" >&2
	else
		mv $n $recyclingBin
	fi
done

exit 0                                    # Exit normally.