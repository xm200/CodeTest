# Banner
echo "------------------------------------------------"
echo ""
echo ""
echo "Welcome! It is CodeTest installer for Linux."
echo ""
echo "You must have administrator rights for installation"
echo ""
echo "This programs will be installed, if they are not installed yet:"
echo "    - gcc/g++ / GNU Compiler Collection"
echo "    - cmake / CMake"
echo "    - ninja / Ninja-build"
echo "    - make"
echo ""
echo ""
echo "------------------------------------------------"
echo ""

if [[ "$EUID" -ne 0 ]]
then
  echo "Run installer as root"
  exit
fi

sub_install() {
  local tmp_path
  tmp_path=$(which "$1")
  if [[ $tmp_path == "" ]]
  then
    echo -n "$1 is not installed. Install it? CodeTest would not work without it. [Y/n]: "
    ans=""
    read -r ans
    if [[ $ans == "Y" || $ans == "" || $ans == "y" ]]
    then
      sudo apt update
      sudo apt install "$2"
    else
      echo "Installation canceled by user."
      exit 0
    fi
  fi
  tmp_path=$(which "$1")
  if [[ $tmp_path == "" ]]
    then
      echo "Something wrong"
      exit 1
  fi
}

sub_install "make" "build-essential"
sub_install "cmake" "cmake"
sub_install "gcc" "gcc"
sub_install "g++" "g++"

cmake -G "Ninja" .
cmake --build .
sudo make install