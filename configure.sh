#!/bin/bash

mac=0

if [ "$(uname)" == "Darwin" ]
then
  mac=1
fi

# Banner
echo "------------------------------------------------"
echo ""
echo ""
echo -n "Welcome! It is CodeTest installer for "
if [ $mac == 1 ]
then
  echo "Linux."
else
  echo "Macos."
fi
echo ""
echo "This programs will be installed, if they are not installed yet:"
echo "    - make"
echo "    - cmake / CMake"
if [ $mac == 1 ]
then
  echo "    - clang"
else
  echo "    - gcc/g++ / GNU Compiler Collection"
fi
echo ""
echo ""
echo "------------------------------------------------"
echo ""

sub_install () {
  local tmp_path
  tmp_path=$(which "$1")
  if [ -z "$tmp_path" ]
  then
    echo -n "$1 is not installed. Install it? CodeTest would not work without it. [Y/n]: "
    if [ "$1" == "brew" ]
    then
      /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    else
      ans=""
      read -r ans
      if [[ "$ans" == "Y" ]] || [ -z "$ans" ] || [[ "$ans" == "y" ]]
      then
        if [ $mac == 0 ]
        then
          sudo apt update
          sudo apt install "$2"
        else
          if [ $mac == 1 ]
          then
            sub_install "brew"
          fi
          brew install "$2"
        fi
      else
        echo "Installation canceled by user."
        exit 0
      fi
    fi
  fi
  tmp_path=$(which "$1")
  if [ -z "$tmp_path" ]
    then
      echo "Something went wrong"
      exit 1
  fi
}


sub_install "make" "build-essential"
sub_install "cmake" "cmake"
if [ $mac == 0 ]
then
  sub_install "gcc" "gcc"
  sub_install "g++" "g++"
fi
cmake .

echo "Done! Now you should run make"